#include "PostProcessingPass.h"
#include "runtime/function/render/RenderPass.h"
#include "runtime/include/EngineWindow.h"
#include "runtime/resource/res_type/common/PostProcessingParameter.h"
#include <cstddef>
#include <memory>
namespace EasyEngine {
    void PostProcessingPass::initialize(){
        float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
        };
        unsigned int quadVBO;
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        
        drawPass->initialize();
        shader->use();
        for (std::shared_ptr<PostProcessingParameter> ppp : drawPass->parameters) {
            if (ppp->parameterName == "framebuffer") {
                framebuffer = std::dynamic_pointer_cast<PostProcessingParameterInt>(ppp)->value;
                continue;
            }
            ppp->setShaderParameter(*shader);
        }

        if (framebuffer == -1) {
            //generate framebuffer
            glGenTextures(1,&textureColorBuffer);
            glBindTexture(GL_TEXTURE_2D,textureColorBuffer);
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,
            EngineWindow::getInstance().viewportWidth,EngineWindow::getInstance().viewportHeight,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glBindTexture(GL_TEXTURE_2D,0);
        
            glGenRenderbuffers(1,&renderbuffer);
            glBindRenderbuffer(GL_RENDERBUFFER,renderbuffer);
            glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,EngineWindow::getInstance().viewportWidth,EngineWindow::getInstance().viewportHeight);
            glBindRenderbuffer(GL_RENDERBUFFER,0);

            glGenFramebuffers(1,&framebuffer);
            glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
            glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,textureColorBuffer,0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,renderbuffer);
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE){
                std::cout<<"frame buffer is not compelete!"<<std::endl;
            }
        
        }        
        glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER,0);

    }
    void PostProcessingPass::draw(Camera& camera){
        glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
        drawPass->draw(camera);
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glClearColor(1,1,1,1);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->use();
        glBindVertexArray(quadVAO);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D,textureColorBuffer);
        glDrawArrays(GL_TRIANGLES,0,6);
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);
    }
    PostProcessingPass::PostProcessingPass(const std::string& shaderPath,std::shared_ptr<RenderPass> drawPass,unsigned int drawPassFrameBuffer):RenderPass(shaderPath),framebuffer(-1),drawPass(drawPass){

    }
}