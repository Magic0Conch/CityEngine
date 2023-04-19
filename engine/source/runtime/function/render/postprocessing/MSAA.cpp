#include "MSAA.h"
#include "glm/fwd.hpp"
#include "runtime/function/render/RenderPass.h"
#include "runtime/function/render/postprocessing/MSAA.h"
#include "runtime/include/EngineWindow.h"
#include "runtime/include/WindowTime.h"
#include "runtime/resource/res_type/components/Model.h"
// #include <GL/gl.h>
// #include <GL/glext.h>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <sys/types.h>

// can't directly use the buffer for other operations like sampling it in a shader. 

namespace EasyEngine {
    MSAA::MSAA(const std::string& shaderpath):RenderPass(shaderpath){
        const string& shaderRootPath = g_global_context.m_config_manager->getShaderFolder();
        screenShader = make_shared<Shader>(PU::getFullPath(shaderRootPath, "postprocessing/screen.vert").c_str(),
        PU::getFullPath(shaderRootPath, "postprocessing/screen.frag").c_str());
        uniformBlockIndex = glGetUniformBlockIndex(screenShader->ID,"Matrices");
        glUniformBlockBinding(screenShader->ID,uniformBlockIndex,0);
    }
    void MSAA::initialize(){
        const int width = EngineWindow::getInstance().viewportWidth;
        const int height = EngineWindow::getInstance().viewportHeight;

            float cubeVertices[] = {
            // positions       
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f
        };
        float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
        };
        // setup cube VAO
        
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        // setup screen VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));



        //create multisampled framebuffer
        glGenFramebuffers(1,&framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
        glGenTextures(1,&textureColorBufferMultiSampled);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,textureColorBufferMultiSampled);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,samples,GL_RGB,EngineWindow::getInstance().viewportWidth,EngineWindow::getInstance().viewportHeight,GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,0);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D_MULTISAMPLE,textureColorBufferMultiSampled,0);
        glGenRenderbuffers(1,&rbo);
        glBindRenderbuffer(GL_RENDERBUFFER,rbo);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER,samples,GL_DEPTH24_STENCIL8,width,height);
        glBindRenderbuffer(GL_RENDERBUFFER,0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rbo); 
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //create intermediateFBO framebuffer
        glGenFramebuffers(1,&intermediateFBO);
        glBindFramebuffer(GL_FRAMEBUFFER,intermediateFBO);
        glGenTextures(1,&screenTexture);
        glBindTexture(GL_TEXTURE_2D,screenTexture);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,screenTexture,0);
        glBindTexture(GL_TEXTURE_2D,0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        screenShader->use();
        screenShader->setValue("texture1",0);
    }

    void MSAA::draw(Camera& camera){
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //use multi sample fbo first
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        shader->use();
        shader->setValue("model", glm::mat4(1.0f));
        
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //copy multisample fbo to normal fbo(shader can't read from multisample fbo)
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
        glBlitFramebuffer(0, 0, EngineWindow::getInstance().viewportWidth, EngineWindow::getInstance().viewportHeight, 0, 0, EngineWindow::getInstance().viewportWidth, EngineWindow::getInstance().viewportHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        
        // draw Screen quad
        screenShader->use();
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, screenTexture); // use the now resolved color attachment as the quad's texture
        glDrawArrays(GL_TRIANGLES, 0, 6);


        glBindVertexArray(0);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    MSAA::~MSAA(){

    }
}
