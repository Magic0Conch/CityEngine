#include "StencilTesting.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/RenderPass.h"
#include "runtime/include/Shader.h"
#include <memory>
#include <string>
namespace EasyEngine {
    StencilTesting::StencilTesting(const std::string& shaderPath):RenderPass(shaderPath){
        const string& shaderRootPath = g_global_context.m_config_manager->getShaderFolder();
        shaderSingleColor = make_shared<Shader>(PU::getFullPath(shaderRootPath, "test/depth_testing.vert").c_str(),
        PU::getFullPath(shaderRootPath, "test/stencil_single_color.frag").c_str());
    }
    void StencilTesting::initialize(){
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS); //GL_ALWAYS GL_NEVER
        glEnable(GL_STENCIL_TEST);

        float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };
        float planeVertices[] = {
            // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
            5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

            5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
            5.0f, -0.5f, -5.0f,  2.0f, 2.0f								
        };
        // cube VAO
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
        // plane VAO
        glGenVertexArrays(1, &planeVAO);
        glGenBuffers(1, &planeVBO);
        glBindVertexArray(planeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);

        cubeTexture = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "default/marble.bmp"));
        floorTexture = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "default/metal.jpg"));

        shader->use();
        shader->setValue("texture1",0);
    }

    void StencilTesting::draw(Camera& camera){
        shader->use();
        glm::mat4 model = glm::mat4(1);
        glm::mat4 view = camera.getCameraPoseMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        shader->setValue("view",view);
        shader->setValue("projection",projection);


        // floor
        glStencilMask(0x00);

        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture->id);
        shader->setValue("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

         // cubes
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS,1,0xff);
        glStencilMask(0xff);

        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture->id); 	
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader->setValue("model",model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader->setValue("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //draw border
        glStencilFunc(GL_NOTEQUAL,1,0xff);
        glStencilMask(0x00); 
        glDisable(GL_DEPTH_TEST);

        shaderSingleColor->use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        model = glm::scale(model, glm::vec3(1.1f,1.1f,1.1f));
        shaderSingleColor->setValue("model",model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.1f,1.1f,1.1f));
        shaderSingleColor->setValue("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);
    }

    StencilTesting::~StencilTesting(){
        glDeleteVertexArrays(1,&cubeVAO);
        glDeleteVertexArrays(1,&planeVAO);
        glDeleteBuffers(1,&cubeVBO);
        glDeleteBuffers(1,&planeVBO);
    }
}