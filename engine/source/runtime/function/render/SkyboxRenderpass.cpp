#include "SkyboxRenderpass.h"
#include "glm/fwd.hpp"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/RenderPass.h"
#include "runtime/include/EngineWindow.h"
#include <cstddef>
#include <memory>
namespace EasyEngine {
    void SkyboxRenderpass::initialize(){


        
        float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
        };
        unsigned int VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        
        vector<std::string> faces({"right.jpg","left.jpg","top.jpg","bottom.jpg","front.jpg","back.jpg"});
        for (std::string& s: faces) {
            s = PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "skybox/skybox/"+s);
        }
        skyboxTexture = make_shared<Texture>(faces,GL_TEXTURE_CUBE_MAP);
    }
    void SkyboxRenderpass::draw(Camera& camera){

        shader->use();
        shader->setValue("projection",camera.getProjectionMatrix());
        shader->setValue("view",glm::mat4(glm::mat3(camera.getCameraPoseMatrix())));
        shader->setValue("skybox",0);
        
        glDepthFunc(GL_LEQUAL);
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glDepthMask(GL_FALSE);
        glBindTexture(GL_TEXTURE_CUBE_MAP,skyboxTexture->id);
        glDrawArrays(GL_TRIANGLES,0,36);
        glBindVertexArray(0);
        // glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        
    }
    SkyboxRenderpass::SkyboxRenderpass(const std::string& shaderPath):RenderPass(shaderPath){}
}