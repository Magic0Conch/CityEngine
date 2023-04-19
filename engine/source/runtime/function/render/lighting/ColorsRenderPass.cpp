#include "ColorsRenderPass.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "runtime/function/render/RenderPass.h"
using namespace EasyEngine;
ColorsRenderPass::ColorsRenderPass(const string& shaderPath)
: RenderPass(shaderPath),objectColor(1.0f,1.0f,1.0f)    
    {
        // Model lightCubeModel(PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"plane.txt")
        //                     , PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"plane_indices.txt"));

        // glGenVertexArrays(1, &lightVAO);
        // glBindVertexArray(lightVAO);
        // //the buffer type of a vertex buffer object is GL_ARRAY_BUFFER.
        // glGenBuffers(1, &lightVBO);
        // //glBufferData is a function specifically targeted to copy user-defined data into the currently bound buffer. 
        // glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
        // glBufferData(GL_ARRAY_BUFFER, lightCubeModel.meshDataSize,lightCubeModel.meshData, GL_STATIC_DRAW);
        
        // glGenBuffers(1, &lightVEO);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightVEO);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, lightCubeModel.indicesSize, lightCubeModel.indices, GL_STATIC_DRAW);

        // //tell OpenGL how to interpret the vertex data;0->set the location of the vertex attribute to 0  layout(location=0);the size of vertex attribute(vec3) stride 
        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        // glEnableVertexAttribArray(0);
        // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
        // glEnableVertexAttribArray(1);
   
        shader->use();
        // shader->setValue("objectColor",objectColor);
        // shader->setValue("lightColor",lightColor);
    }

    void ColorsRenderPass::setModelMatrix(const glm::mat4& modelMatrix){
        // shader->setValue("model",glm::translate(modelMatrix, lightPos));
    }
    void ColorsRenderPass::setViewMatrix(const glm::mat4& view){
        // shader->setValue("view", view);
    }

    void ColorsRenderPass::setProjectionMatrix(const glm::mat4& projection){
        // shader->setValue("projection", projection);
    }

    void ColorsRenderPass::draw(Camera& camera){
        shader->use();
        // shader->setValue("objectColor",objectColor);
        // shader->setValue("lightColor",lightColor);

        int cubePositionsLen = sceneConfig["lightPositions"].size();
        for (int i = 0; i < cubePositionsLen; ++i) {
            model = glm::mat4(1.0f);
            Json::Value cubePositionRaw = sceneConfig["lightPositions"][i];
            glm::vec3 worldPosition = glm::vec3(cubePositionRaw[0].asFloat(), cubePositionRaw[1].asFloat(), cubePositionRaw[2].asFloat());
            model = glm::translate(model, worldPosition);
            model = glm::scale(model, glm::vec3(0.2f));
            setModelMatrix(model);
            setViewMatrix(camera.getCameraPoseMatrix());
            setProjectionMatrix(camera.getProjectionMatrix());
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    void ColorsRenderPass::initialize(){
        JsonManipulation jsonManipulation(PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"scene.json"));
        sceneConfig = jsonManipulation.getJsonData();
    }