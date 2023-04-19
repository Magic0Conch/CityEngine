#pragma once
#include "runtime/function/render/RenderPass.h"
#include <memory>
#include <string.h>
#include <glad/glad.h>
#include "runtime/resource/res_type/components/Texture.h"
namespace EasyEngine {
    class StencilTesting:public RenderPass{
        private:
            shared_ptr<Shader> shaderSingleColor;
        public:
            unsigned int cubeVAO, cubeVBO;
            unsigned int planeVAO, planeVBO;        
            shared_ptr<Texture> cubeTexture,floorTexture;

            StencilTesting(const std::string& shaderPath);
            virtual void draw(Camera& camera);
            virtual void initialize();
            virtual ~StencilTesting();

    };
}
