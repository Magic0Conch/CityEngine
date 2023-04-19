#include "GeometryRenderpass.h"
#include "runtime/function/render/RenderPass.h"
#include "runtime/include/EngineWindow.h"
// #include <GL/gl.h>
#include <cstddef>
#include <iostream>
#include <memory>
#include <sys/types.h>

using namespace EasyEngine;

GeometryRenderpass::GeometryRenderpass(const std::string shaderPath):RenderPass(shaderPath),width(EngineWindow::getInstance().viewportWidth),height(EngineWindow::getInstance().viewportHeight){

}

GeometryRenderpass::~GeometryRenderpass(){

}

void GeometryRenderpass::initialize(){

    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    glGenTextures(1,&gPosition);
    glBindTexture(GL_TEXTURE_2D,gPosition);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,width,height,0,GL_RGBA,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,gPosition,0);

    glGenTextures(1,&gNormal);
    glBindTexture(GL_TEXTURE_2D,gNormal);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,width,height,0,GL_RGBA,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,gNormal,0);

    glGenTextures(1,&gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D,gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,width,height,0,GL_RGBA,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT2,GL_TEXTURE_2D,gAlbedoSpec,0);

    unsigned int attachements[3] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3,attachements);

    glGenRenderbuffers(1,&gRbo);
    glBindRenderbuffer(GL_RENDERBUFFER,gRbo);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,width,height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,gRbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: gBuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    Model backpackModel(PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"backpack/backpack.obj"));
    models.emplace_back(backpackModel);
    
    objectPositions.push_back(glm::vec3(-3.0, -0.5, -3.0));
    objectPositions.push_back(glm::vec3( 0.0, -0.5, -3.0));
    objectPositions.push_back(glm::vec3( 3.0, -0.5, -3.0));
    objectPositions.push_back(glm::vec3(-3.0, -0.5,  0.0));
    objectPositions.push_back(glm::vec3( 0.0, -0.5,  0.0));
    objectPositions.push_back(glm::vec3( 3.0, -0.5,  0.0));
    objectPositions.push_back(glm::vec3(-3.0, -0.5,  3.0));
    objectPositions.push_back(glm::vec3( 0.0, -0.5,  3.0));
    objectPositions.push_back(glm::vec3( 3.0, -0.5,  3.0));
}

void GeometryRenderpass::draw(Camera &camera){
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->use();
        if (glGetError()) {
            cout<<glGetError()<<endl;
        }


    for (unsigned int i = 0; i < objectPositions.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, objectPositions[i]);
        model = glm::scale(model, glm::vec3(0.25f));
        shader->setValue("model", model);
        models[0].draw(*shader);
    }
    if (glGetError()) {
        cout<<glGetError()<<endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER,0);
}
