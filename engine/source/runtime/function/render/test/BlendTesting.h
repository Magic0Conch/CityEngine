#pragma once
#include "runtime/function/render/RenderPass.h"
#include <memory>
#include <string.h>
#include <glad/glad.h>
#include "runtime/resource/res_type/components/Texture.h"
namespace EasyEngine {
    class BlendTesting:public RenderPass{
        private:            
            vector<glm::vec3> vegetation;

        public:
            unsigned int cubeVAO, cubeVBO;
            unsigned int grassVAO, grassVBO;
            unsigned int planeVAO, planeVBO;        
            shared_ptr<Texture> cubeTexture,floorTexture,grassTexture;

            BlendTesting(const std::string& shaderPath);
            virtual void draw(Camera& camera);
            virtual void initialize();
            virtual ~BlendTesting();

    };
}
