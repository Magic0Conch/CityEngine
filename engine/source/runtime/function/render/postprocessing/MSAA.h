#pragma once
#include "glm/fwd.hpp"
#include "runtime/function/render/RenderPass.h"
#include "runtime/resource/res_type/components/Model.h"
#include <memory>
#include <sys/types.h>
#include <vector>
namespace EasyEngine {
    class MSAA: public RenderPass{
        private:
        public:
            const int samples = 4;
            unsigned int VAO,framebuffer,textureColorBufferMultiSampled,multisampledFBO,rbo,intermediateFBO,screenTexture;
            unsigned int cubeVAO, cubeVBO;
            unsigned int quadVAO, quadVBO;

            shared_ptr<Shader> screenShader;
        public:
            MSAA(const std::string& shaderPath);
            virtual void initialize();
            virtual void draw(Camera& camera);
            virtual ~MSAA();

    };
}
