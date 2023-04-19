#include "Bloom.h"
#include "glm/fwd.hpp"
#include "runtime/function/render/RenderPass.h"
#include "runtime/function/render/postprocessing/MSAA.h"
#include "runtime/include/EngineWindow.h"
#include "runtime/include/Shader.h"
#include "runtime/include/WindowTime.h"
#include "runtime/resource/res_type/components/Model.h"
#include "runtime/resource/res_type/components/Texture.h"
// #include <GL/gl.h>
// #include <GL/glext.h>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <string>
#include <sys/types.h>


namespace EasyEngine {
    Bloom::Bloom():RenderPass("common/triangle","test/bloom_test"){
        const string& shaderRootPath = g_global_context.m_config_manager->getShaderFolder();
        shaderLight = make_shared<Shader>(PU::getFullPath(shaderRootPath, "common/triangle.vert").c_str(),
        PU::getFullPath(shaderRootPath, "test/bloom_light.frag").c_str());
        uniformBlockIndex = glGetUniformBlockIndex(shaderLight->ID,"Matrices");
        glUniformBlockBinding(shaderLight->ID,uniformBlockIndex,0);

        shaderBlur = make_shared<Shader>(PU::getFullPath(shaderRootPath, "postprocessing/screen.vert").c_str(),
        PU::getFullPath(shaderRootPath, "postprocessing/GaussianBlur.frag").c_str());

        shaderBloomFinal = make_shared<Shader>(PU::getFullPath(shaderRootPath, "postprocessing/screen.vert").c_str(),
        PU::getFullPath(shaderRootPath, "postprocessing/bloom_final.frag").c_str());

        
    }
    void Bloom::initialize(){
        const int width = EngineWindow::getInstance().viewportWidth;
        const int height = EngineWindow::getInstance().viewportHeight;
        

        //load textures
        const std::string dataRootPath = g_global_context.m_config_manager->getTextureFolder();
        textureBox = make_shared<Texture>(PU::getFullPath(dataRootPath, "default/steel_border_box.bmp"));
        textureFloor = make_shared<Texture>(PU::getFullPath(dataRootPath, "default/woodfloor.jpg"));

        //configure hdr FBO
        glGenFramebuffers(1,&hdrFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

        for (unsigned int i = 0; i<2; ++i) {
            colorBuffers[i] = make_shared<Texture>(width,height,4,HDR);
            glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+i,GL_TEXTURE_2D,colorBuffers[i]->id,0);
        }

        unsigned int rboDepth;
        glGenRenderbuffers(1,&rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER,rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,width,height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,rboDepth);
        
        unsigned int attachements[2] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1};
        glDrawBuffers(2,attachements);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //configure ping pong framebuffers
        glGenFramebuffers(2,pingpongFBOs);
        for (unsigned int i = 0; i<2; ++i) {
            glBindFramebuffer(GL_FRAMEBUFFER,pingpongFBOs[i]);
            pingpongBuffers[i] = make_shared<Texture>(width,height,4,HDR);
            glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,pingpongBuffers[i]->id,0);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << endl;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }        


        //lighting
        lightPositions.push_back(glm::vec3( 0.0f, 0.5f,  1.5f));
        lightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
        lightPositions.push_back(glm::vec3( 3.0f, 0.5f,  1.0f));
        lightPositions.push_back(glm::vec3(-.8f,  2.4f, -1.0f));
        // colors
        lightColors.push_back(glm::vec3(5.0f,   5.0f,  5.0f));
        lightColors.push_back(glm::vec3(10.0f,  0.0f,  0.0f));
        lightColors.push_back(glm::vec3(0.0f,   0.0f,  15.0f));
        lightColors.push_back(glm::vec3(0.0f,   5.0f,  0.0f));

        shader->use();
        shader->setValue("diffuseTexture",0);

        shaderBlur->use();
        shaderBlur->setValue("image",0);
        shaderBloomFinal->use();
        shaderBloomFinal->setValue("scene",0);
        shaderBloomFinal->setValue("blurLightTexture",1);
    }

    void Bloom::draw(Camera& camera){
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        //1.
        glBindFramebuffer(GL_FRAMEBUFFER,hdrFBO);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        shader->use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textureFloor->id);
        for (unsigned int i = 0; i<lightPositions.size(); ++i) {
            shader->setValue("lights[" + std::to_string(i)+"].Position",lightPositions[i]);
            shader->setValue("lights[" + std::to_string(i)+"].Color",lightColors[i]);
        }

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
        model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
        shader->setValue("model", model);
        renderCube();
        // then create multiple cubes as the scenery
        glBindTexture(GL_TEXTURE_2D, textureBox->id);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
        model = glm::scale(model, glm::vec3(0.5f));
        shader->setValue("model", model);
        renderCube();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
        model = glm::scale(model, glm::vec3(0.5f));
        shader->setValue("model", model);
        renderCube();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
        model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        shader->setValue("model", model);
        renderCube();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
        model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        model = glm::scale(model, glm::vec3(1.25));
        shader->setValue("model", model);
        renderCube();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
        model = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        shader->setValue("model", model);
        renderCube();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
        model = glm::scale(model, glm::vec3(0.5f));
        shader->setValue("model", model);
        renderCube();

        shaderLight->use();
        
        for (unsigned int i = 0; i < lightPositions.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(lightPositions[i]));
            model = glm::scale(model, glm::vec3(0.25f));
            shaderLight->setValue("model", model);
            shaderLight->setValue("lightColor", lightColors[i]);
            renderCube();
        }
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        // 2: blur bright fragments
        bool horizontal=true,first_iteration = true;
        unsigned int amount = 10;
        shaderBlur->use();
        for (unsigned int i = 0; i<amount; ++i) {
            glBindFramebuffer(GL_FRAMEBUFFER,pingpongFBOs[horizontal]);
            shaderBlur->setValue("horizontal",horizontal);
            glBindTexture(GL_TEXTURE_2D,first_iteration?colorBuffers[1]->id:pingpongBuffers[!horizontal]->id);
            renderQuad();
            horizontal = !horizontal;
            if (first_iteration) {
                first_iteration = false;
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER,0);

        //3. add
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderBloomFinal->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[0]->id);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffers[!horizontal]->id);
        // shaderBloomFinal->setValue("bloom",1);
        shaderBloomFinal->setValue("exposure",1.0f);
        renderQuad();
    }
    void Bloom::renderCube()
    {
        // initialize (if necessary)
        if (cubeVAO == 0)
        {
            float vertices[] = {
                // back face
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
                1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
                // front face
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
                1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
                1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
                // left face
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
                -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
                // right face
                1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
                1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
                1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
                1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
                1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
                1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
                // bottom face
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
                1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
                // top face
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
                1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
                1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
                1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
                -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
            };
            glGenVertexArrays(1, &cubeVAO);
            glGenBuffers(1, &cubeVBO);
            // fill buffer
            glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            // link vertex attributes
            glBindVertexArray(cubeVAO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        // render Cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    void Bloom::renderQuad()
    {
        if (quadVAO == 0)
        {
            float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            // setup plane VAO
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }

    Bloom::~Bloom(){

    }
}
