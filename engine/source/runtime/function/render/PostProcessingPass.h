#pragma once
#include "runtime/function/render/RenderPass.h"
#include "runtime/resource/res_type/components/Camera.h"
#include <memory>
#include <sys/types.h>
namespace EasyEngine {
    class PostProcessingPass:public RenderPass{
        private:
            unsigned int framebuffer;
            unsigned int textureColorBuffer;
            unsigned int renderbuffer;
            unsigned int quadVAO;
            std::shared_ptr<RenderPass> drawPass;
            
            // bool withFramebuffer = false;

        public:
            virtual void initialize();
            virtual void draw(Camera& camera);
            PostProcessingPass(const std::string& shaderPath, std::shared_ptr<RenderPass> drawPass,unsigned int drawPassFrameBuffer = -1);

    };

}