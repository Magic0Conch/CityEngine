#include "GammaCorrectionTesting.h"
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
#include "runtime/include/InputHandler.h"
#include "runtime/resource/res_type/components/Texture.h"


//glEnable(GL_FRAMEBUFFER_SRGB); 
namespace EasyEngine {
    GammaCorrectionTesting::GammaCorrectionTesting(const std::string& shaderpath):RenderPass(shaderpath){}
    void GammaCorrectionTesting::initialize(){
        const int width = EngineWindow::getInstance().viewportWidth;
        const int height = EngineWindow::getInstance().viewportHeight;
        float planeVertices[] = {
            // positions            // normals         // texcoords
            10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
            -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
            -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

            10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
            -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
            10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
        };
        glGenVertexArrays(1, &planeVAO);
        glGenBuffers(1, &planeVBO);
        glBindVertexArray(planeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindVertexArray(0);

        // load textures
        // -------------
        floorTexture = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "default/woodfloor.jpg"),GL_TEXTURE_2D,DEFAULT);
        floorTextureGammaCorrected = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "default/woodfloor.jpg"),GL_TEXTURE_2D,GAMMACORRECTION);

        shader->use();
        shader->setValue("texture1",0);

        
    
    }

    void GammaCorrectionTesting::draw(Camera& camera){
        glm::vec3 lightPositions[] = {
            glm::vec3(-3.0f, 0.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3 (1.0f, 0.0f, 0.0f),
            glm::vec3 (3.0f, 0.0f, 0.0f)
        };
        glm::vec3 lightColors[] = {
            glm::vec3(0.25),
            glm::vec3(0.50),
            glm::vec3(0.75),
            glm::vec3(1.00)
        };
        shader->use();
        glUniform3fv(glGetUniformLocation(shader->ID,"lightPositions"),4,&lightPositions[0][0]);
        glUniform3fv(glGetUniformLocation(shader->ID,"lightColors"),4,&lightColors[0][0]);
        shader->setValue("viewPos",camera.cameraPosition);
        gammaEnabled = InputHandler::getInstance().Gpressed;
        shader->setValue("gamma",gammaEnabled);
        
        glBindVertexArray(planeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gammaEnabled ? floorTextureGammaCorrected->id : floorTexture->id);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // std::cout << (gammaEnabled ? "Gamma enabled" : "Gamma disabled") << std::endl;

    
    }
    GammaCorrectionTesting::~GammaCorrectionTesting(){

    }
}
