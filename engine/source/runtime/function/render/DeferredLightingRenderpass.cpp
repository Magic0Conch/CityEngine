#include "DeferredLightingRenderpass.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "runtime/function/render/RenderPass.h"
#include "runtime/function/render/basic_shape/RenderShape.h"
#include "runtime/include/EngineWindow.h"
#include "runtime/include/Shader.h"
// #include <GL/gl.h>
#include <cstddef>
#include <iostream>
#include <memory>
#include <sys/types.h>

using namespace EasyEngine;

DeferredLightingRenderpass::DeferredLightingRenderpass(const std::string vertPath,const std::string fragPath,shared_ptr<GeometryRenderpass> grsp):width(EngineWindow::getInstance().viewportWidth),height(EngineWindow::getInstance().viewportHeight),
grsp(grsp),RenderPass(vertPath,fragPath){
    shaderLightBox = std::make_shared<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "lighting/color.vert").c_str()
               ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "lighting/color.frag").c_str());
    uniformBlockIndex = glGetUniformBlockIndex(shaderLightBox->ID,"Matrices");
    glUniformBlockBinding(shaderLightBox->ID,uniformBlockIndex,0);
}

DeferredLightingRenderpass::~DeferredLightingRenderpass(){

}

void DeferredLightingRenderpass::initialize(){
    gPosition = grsp->gPosition;
    gNormal = grsp->gNormal;
    gAlbedoSpec = grsp->gAlbedoSpec;
    gBuffer = grsp->gBuffer;
    gRbo = grsp->gRbo;
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

    shader->use();
    shader->setValue("gPosition", 0);
    shader->setValue("gNormal", 1);
    shader->setValue("gAlbedoSpec", 2);
}

void DeferredLightingRenderpass::draw(Camera &camera){
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    
    //senduniforms
    for (unsigned int i = 0; i < lightPositions.size(); i++)
    {
        shader->setValue("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
        shader->setValue("lights[" + std::to_string(i) + "].Color", lightColors[i]);
        // update attenuation parameters and calculate radius
        const float constant = 1.0f; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
        const float linear = 0.7f;
        const float quadratic = 1.8f;
        shader->setValue("lights[" + std::to_string(i) + "].Linear", linear);
        shader->setValue("lights[" + std::to_string(i) + "].Quadratic", quadratic);
        // then calculate radius of light volume/sphere
        const float maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);
        float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
        shader->setValue("lights[" + std::to_string(i) + "].Radius", radius);
    }

    shader->setValue("viewPos",camera.cameraPosition);
    renderQuad();
    glBindFramebuffer(GL_READ_FRAMEBUFFER,gBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    shaderLightBox->use();
    for (unsigned int i = 0; i < lightPositions.size(); i++)
    {
        model = glm::mat4(1);
        model = glm::translate(model, lightPositions[i]);
        model = glm::scale(model, glm::vec3(0.03));
        shaderLightBox->setValue("lightColor",lightColors[i]);
        shaderLightBox->setValue("model",model);
        shaderLightBox->setValue("objectColor",glm::vec3(1.0f));
        renderCube();
    }
}

void DeferredLightingRenderpass::renderQuad(){
    RenderShape::instance().renderQuad();
}

void DeferredLightingRenderpass::renderCube(){
    RenderShape::instance().renderCube();
}