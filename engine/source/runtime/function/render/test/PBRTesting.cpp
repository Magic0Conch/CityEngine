#include "runtime/core/base/macro.h"
#include "PBRTesting.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "runtime/function/render/RenderPass.h"
#include "runtime/function/render/basic_shape/RenderShape.h"
#include "runtime/include/EngineWindow.h"
#include "runtime/include/Shader.h"
#include "runtime/resource/res_type/components/Texture.h"
// #include <GL/gl.h>
// #include <GL/glext.h>
#include <cstddef>
#include <iostream>
#include <memory>
#include <opencv2/imgcodecs.hpp>
#include <sys/types.h>

using namespace EasyEngine;

PBRTesting::PBRTesting(const std::string shaderPath):RenderPass(shaderPath),width(EngineWindow::getInstance().viewportWidth),height(EngineWindow::getInstance().viewportHeight){
    glCheckError();
    
    shaderEquirect = std::make_shared<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "common/equirect_cubemap.vert").c_str()
            ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "common/equirect_cubemap.frag").c_str());
    uniformBlockIndex = glGetUniformBlockIndex(shaderEquirect->ID,"Matrices");
    glUniformBlockBinding(shaderEquirect->ID,uniformBlockIndex,0);
        glCheckError();
    shaderSkybox = std::make_shared<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "skybox.vert").c_str()
            ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "skybox.frag").c_str());
    shaderIrradiance = std::make_shared<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "common/equirect_cubemap.vert").c_str()
            ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "test/cubemap_convolution.frag").c_str());
        glCheckError();
}

PBRTesting::~PBRTesting(){

}

void PBRTesting::initialize(){
    glCheckError();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); 
    glCheckError();

    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
    glCheckError();

    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (unsigned int i = 0; i < 6; ++i){
        // note that we store each face with 16 bit floating point values
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glCheckError();

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCheckError();

    cv::Mat data = ImageProcessing::readImageByPath(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "skybox/newport_loft.hdr"),IMREAD_UNCHANGED,true);
    glGenTextures(1,&textureHDR);
    glBindTexture(GL_TEXTURE_2D,textureHDR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, data.cols, data.rows, 0, GL_BGR, GL_FLOAT, data.data); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data.release();
    glCheckError();

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f),1.0f,0.1f,10.0f);
    glm::mat4 captureViews[] = {
        glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(-1.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec3(0.0f,0.0f,1.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f), glm::vec3(0.0f,0.0f,-1.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f,-1.0f,0.0f)),
        glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f,-1.0f,0.0f)),
    };
    shaderEquirect->use();
    shaderEquirect->setValue("equirectangularMap",0);
    shaderEquirect->setValue("projection",captureProjection);
    glCheckError();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textureHDR);
    glViewport(0,0,512,512);
    glBindFramebuffer(GL_FRAMEBUFFER,captureFBO);
    for (unsigned int i = 0; i<6; ++i) {
        shaderEquirect->setValue("view",captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,envCubemap,0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ERROR::FRAMEBUFFER:: captureFBO is not complete!" << std::endl;
        }
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        RenderShape::instance().renderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);


    glCheckError();

    const unsigned int NR_LIGHTS = 1;
    for (unsigned int i = 0; i < NR_LIGHTS; i++)
    {
        lightPositions.push_back(glm::vec3(0.0, 0.0, 0.0));
        lightColors.push_back(glm::vec3(150, 150, 150));
    }
    shader->use();
    shader->setValue("albedoMap",0);
    shader->setValue("normalMap",1);
    shader->setValue("metallicMap",2);
    shader->setValue("roughnessMap",3);
    shader->setValue("aoMap",4);
    glCheckError();

    albedoMap = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "default/rustediron/rustediron2_basecolor.png"),GL_TEXTURE_2D,DEFAULT);
    normalMap = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "default/rustediron/rustediron2_normal.png"),GL_TEXTURE_2D,DEFAULT);
    metallicMap = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "default/rustediron/rustediron2_metallic.png"),GL_TEXTURE_2D,DEFAULT);
    roughnessMap = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "default/rustediron/rustediron2_roughness.png"),GL_TEXTURE_2D,DEFAULT);
    aoMap = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "default/rustediron/rustediron2_ao.png"),GL_TEXTURE_2D,DEFAULT);
    shaderSkybox->use();
    shaderSkybox->setValue("skybox",0);
    glCheckError();

    glGenTextures(1,&irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (unsigned int i = 0; i<6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, 
                    GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCheckError();

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glCheckError();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
    glCheckError();
    
    shaderIrradiance->use();
    shaderIrradiance->setValue("environmentMap",0);
    shaderIrradiance->setValue("projection",captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP,envCubemap);
    glViewport(0, 0, 32, 32);
    glBindFramebuffer(GL_FRAMEBUFFER,captureFBO);
    for (unsigned int i = 0; i < 6; ++i){
        shaderIrradiance->setValue("view",captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ERROR::FRAMEBUFFER:: captureFBO is not complete!" << std::endl;
        }
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        RenderShape::instance().renderCube();
        
    }
        // shaderIrradiance->setValue("view",glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f)));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                            GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, irradianceMap, 0);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glViewport(0,0,width,height);
}

void PBRTesting::draw(Camera &camera){
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, albedoMap->id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalMap->id);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, metallicMap->id);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, roughnessMap->id);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, aoMap->id);
    glm::mat4 model = glm::mat4(1.0f);

    for (unsigned int i = 0; i < lightPositions.size(); i++){
        shader->setValue("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
        shader->setValue("lights[" + std::to_string(i) + "].Color", lightColors[i]);
        const float constant = 1.0f; 
        const float linear = 0.7f;
        const float quadratic = 1.8f;
        shader->setValue("lights[" + std::to_string(i) + "].Linear", linear);
        shader->setValue("lights[" + std::to_string(i) + "].Quadratic", quadratic);
    }

    for (int row = 0; row < nrRows; ++row){
        for (int col = 0; col < nrColumns; ++col){
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(
                (float)(col - (nrColumns*1.0 / 2)) * spacing,
                (float)(row - (nrRows*1.0 / 2)) * spacing,
                0.0f
            ));
            shader->setValue("model", model);
            RenderShape::instance().renderSphere();
        }
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP,irradianceMap);
    shaderSkybox->use();
    RenderShape::instance().renderCube();
    
}
