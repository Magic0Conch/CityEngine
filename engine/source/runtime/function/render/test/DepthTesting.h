#pragma once
#include "runtime/function/render/RenderPass.h"
#include <memory>
#include <string.h>
#include <glad/glad.h>
#include "runtime/resource/res_type/components/Texture.h"
namespace EasyEngine {
    class DepthTesting:public RenderPass{
        public:
            unsigned int cubeVAO, cubeVBO;
            unsigned int planeVAO, planeVBO;        
            shared_ptr<Texture> cubeTexture,floorTexture;

            DepthTesting(const std::string& shaderPath);
            virtual void draw(Camera& camera);
            virtual void initialize();
            virtual ~DepthTesting();

    };
}
