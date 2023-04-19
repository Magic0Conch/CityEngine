#include "SSAOTest.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "runtime/core/math/Math.h"
#include "runtime/function/render/RenderPass.h"
#include "runtime/function/render/basic_shape/RenderShape.h"
#include "runtime/include/EngineWindow.h"
#include "runtime/include/InputHandler.h"
#include <cstddef>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <sys/types.h>

using namespace EasyEngine;

SSAOTest::SSAOTest(const std::string vertPath,const std::string fragPath,shared_ptr<GeometryRenderpass> grsp_view):width(EngineWindow::getInstance().viewportWidth),
height(EngineWindow::getInstance().viewportHeight),grsp_view(grsp_view),RenderPass(vertPath,fragPath){
    shaderBlur = std::make_shared<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "postprocessing/screen.vert").c_str()
               ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "postprocessing/SSAO_blur.frag").c_str());
    shaderLighting = std::make_shared<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "postprocessing/screen.vert").c_str()
    ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "test/SSAO_lighting.frag").c_str());
    uniformBlockIndex = glGetUniformBlockIndex(shaderLighting->ID,"Matrices");
    glUniformBlockBinding(shaderLighting->ID,uniformBlockIndex,0);
}

SSAOTest::~SSAOTest(){

}

void SSAOTest::initialize(){
    generateSsaoParameters();
    const unsigned int NR_LIGHTS = 32;
    srand(13);
    for (unsigned int i = 0; i < NR_LIGHTS; i++)
    {
        // calculate slightly random offsets
        float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
        float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
        float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
        lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
        // also calculate random color
        float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
        float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
        float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
        lightColors.push_back(glm::vec3(rColor, gColor, bColor));
    }

    gPosition = grsp_view->gPosition;
    gNormal = grsp_view->gNormal;
    gAlbedoSpec = grsp_view->gAlbedoSpec;
    gBuffer = grsp_view->gBuffer;
    gRbo = grsp_view->gRbo;

    glGenTextures(1,&noiseTexture);
    glBindTexture(GL_TEXTURE_2D,noiseTexture);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,4,4,0,GL_RGB,GL_FLOAT,&ssaoNoises[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 

    glGenFramebuffers(1, &ssaoFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
        glGenTextures(1,&ssaoColorBuffer);
        glBindTexture(GL_TEXTURE_2D,ssaoColorBuffer);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RED,width,height,0,GL_RED,GL_FLOAT,NULL);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,ssaoColorBuffer,0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: ssaoFBO is not complete!" << std::endl;
    }

    glGenFramebuffers(1,&ssaoBlurFBO);
    glBindFramebuffer(GL_FRAMEBUFFER,ssaoBlurFBO);
    glGenTextures(1, &ssaoColorBufferBlur);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: ssaoBlurFBO is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER,0);
    shader->use();
    shader->setValue("gPosition",0);
    shader->setValue("gNormal",1);
    shader->setValue("texNoise",2);
    shaderBlur->use();
    shaderBlur->setValue("screenTexture",0);
    shaderLighting->use();
    shaderLighting->setValue("gPosition",0);
    shaderLighting->setValue("gNormal",1);
    shaderLighting->setValue("gAlbedoSpec",2);
    shaderLighting->setValue("ssao",3);
}

void SSAOTest::draw(Camera &camera){
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
        glClear(GL_COLOR_BUFFER_BIT);
        shader->use();
        for (int i = 0; i<ssaoKernels.size(); ++i) {
            shader->setValue("samples["+ to_string(i)+"]",ssaoKernels[i]); 
        }
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        RenderShape::instance().renderQuad();
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    glBindFramebuffer(GL_FRAMEBUFFER,ssaoBlurFBO);
    glClear(GL_COLOR_BUFFER_BIT);
    shaderBlur->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,ssaoColorBuffer);
    RenderShape::instance().renderQuad();

    glBindFramebuffer(GL_FRAMEBUFFER,0);
    //lighting
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    shaderLighting->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,gAlbedoSpec);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D,ssaoColorBufferBlur);
    shaderLighting->setValue("viewPos",camera.cameraPosition);
    for (unsigned int i = 0; i < lightPositions.size(); i++)
    {
        shaderLighting->setValue("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
        shaderLighting->setValue("lights[" + std::to_string(i) + "].Color", lightColors[i]);
        const float constant = 1.0f; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
        const float linear = 0.7f;
        const float quadratic = 1.8f;
        shaderLighting->setValue("lights[" + std::to_string(i) + "].Linear", linear);
        shaderLighting->setValue("lights[" + std::to_string(i) + "].Quadratic", quadratic);
        const float maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);
        float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
        shaderLighting->setValue("lights[" + std::to_string(i) + "].Radius", radius);
    }
    shaderLighting->setValue("ssaoEnabled",InputHandler::getInstance().Gpressed);
    RenderShape::instance().renderQuad();
}

void SSAOTest::generateSsaoParameters(){
    std::uniform_real_distribution<float> randomFloats(0.0,1.0);
    std::default_random_engine generator;
    for (int i = 0; i<64; ++i) {
        glm::vec3 sample(
            randomFloats(generator)*2.0-1.0,
            randomFloats(generator)*2.0-1.0,
            randomFloats(generator)
        );
        sample = glm::normalize(sample);
        float scale = i * 1.0/64;
        scale = Math::lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        ssaoKernels.emplace_back(sample);
    }

    for (int i = 0; i<16; ++i) {
        glm::vec3 noise(
            randomFloats(generator)*2.0-1.0,
            randomFloats(generator)*2.0-1.0,
            0.0f
        );      
        ssaoNoises.emplace_back(noise);  
    }


}