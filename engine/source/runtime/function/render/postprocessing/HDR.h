#pragma once
#include "glm/fwd.hpp"
#include "runtime/function/render/RenderPass.h"
#include "runtime/resource/res_type/components/Model.h"
#include <memory>
#include <sys/types.h>
#include <vector>
namespace EasyEngine {
    class HDR: public RenderPass{
        private:
        public:
            unsigned int VAO,framebuffer,screenTexture;
            unsigned int quadVAO, quadVBO;

            shared_ptr<Shader> screenShader;
        public:
            HDR(const std::string& shaderPath);
            virtual void initialize();
            virtual void draw(Camera& camera);
            virtual ~HDR();

    };
}
