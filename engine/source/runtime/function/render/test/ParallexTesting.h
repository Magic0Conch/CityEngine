#pragma once
#include "glm/fwd.hpp"
#include "runtime/function/render/RenderPass.h"
#include <memory>
#include <string.h>
#include <glad/glad.h>
#include "runtime/resource/res_type/components/Texture.h"
namespace EasyEngine {
    class ParallexTesting:public RenderPass{
        private:            
            vector<glm::vec3> vegetation;
            glm::vec3 lightPos;
            shared_ptr<Shader> colorShader;
        public:
            unsigned int cubeVAO, cubeVBO;
            unsigned int grassVAO, grassVBO;
            unsigned int planeVAO, planeVBO;        
            shared_ptr<Texture> cubeTexture,cubenormalTexture,depthTexture;
            glm::mat2x3 getTB(glm::vec3 pos1,glm::vec3 pos2, glm::vec3 pos3 ,glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3);
            ParallexTesting(const std::string& shaderPath);
            virtual void draw(Camera& camera);
            virtual void initialize();
            virtual ~ParallexTesting();

    };
}
