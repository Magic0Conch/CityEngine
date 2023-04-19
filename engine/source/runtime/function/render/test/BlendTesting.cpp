#include "BlendTesting.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/RenderPass.h"
#include "runtime/include/Shader.h"
// #include <GL/gl.h>
#include <map>
#include <memory>
#include <string>
namespace EasyEngine {
    BlendTesting::BlendTesting(const std::string& shaderPath):RenderPass(shaderPath){}
    void BlendTesting::initialize(){

        float cubeVertices[] = {
            // Back face
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
            // Front face
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
            // Left face
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
            // Right face
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
            // Bottom face
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
            // Top face
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
        };
        float planeVertices[] = {
            // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)

            5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
            -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,

            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
            5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            5.0f, -0.5f, -5.0f,  2.0f, 2.0f								
        };
        float transparentVertices[] = {
            // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
            1.0f,  0.5f,  0.0f,  1.0f,  0.0f
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
        //grassVAO
        glGenVertexArrays(1,&grassVAO);
        glGenBuffers(1,&grassVBO);
        glBindVertexArray(grassVAO);
        glBindBuffer(GL_ARRAY_BUFFER,grassVBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(transparentVertices),&transparentVertices,GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);


        vegetation.push_back(glm::vec3(-1.5f,  0.0f, -0.48f));
        vegetation.push_back(glm::vec3( 1.5f,  0.0f,  0.51f));
        vegetation.push_back(glm::vec3( 0.0f,  0.0f,  0.7f));
        vegetation.push_back(glm::vec3(-0.3f,  0.0f, -2.3f));
        vegetation.push_back(glm::vec3( 0.5f,  0.0f, -0.6f));  

        cubeTexture = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "default/marble.bmp"));
        floorTexture = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "default/metal.jpg"));
        grassTexture = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "default/grass.png"));
        glBindTexture(GL_TEXTURE_2D,grassTexture->id);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        shader->use();
        shader->setValue("texture1",0);

    }

    void BlendTesting::draw(Camera& camera){
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS); //GL_ALWAYS GL_NEVER
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ZERO);

        glCullFace(GL_BACK);  
        glFrontFace(GL_CCW);  //CW is recognised as front face
        // glBlendEquation(GL_ADD);

        // glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->use();
        glm::mat4 model = glm::mat4(1);
        glm::mat4 view = camera.getCameraPoseMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        // floor
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture->id);
        shader->setValue("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

         // cubes
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

        //grass
        glBindVertexArray(grassVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grassTexture->id);
        std::map<float, glm::vec3> sorted;
        //solve the depth test problem which cause occlusion between grass.
        for (glm::vec3& pos : vegetation) {
            sorted[glm::distance(pos, camera.cameraPosition)]=pos;
        }
        for (map<float,glm::vec3>::reverse_iterator it = sorted.rbegin(); it!=sorted.rend(); it++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            shader->setValue("model",model);
            glDrawArrays(GL_TRIANGLES,0,36);
        }
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glBindVertexArray(0);

    }

    BlendTesting::~BlendTesting(){
        glDeleteVertexArrays(1,&cubeVAO);
        glDeleteVertexArrays(1,&planeVAO);
        glDeleteBuffers(1,&cubeVBO);
        glDeleteBuffers(1,&planeVBO);
    }
}