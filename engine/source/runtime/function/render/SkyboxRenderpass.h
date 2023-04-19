#pragma once
#include "runtime/function/render/RenderPass.h"
#include "runtime/resource/res_type/components/Camera.h"
#include "runtime/resource/res_type/components/Texture.h"
#include <memory>
#include <sys/types.h>
namespace EasyEngine {
    class SkyboxRenderpass:public RenderPass{
        private:
            unsigned int VAO;
            shared_ptr<Texture> skyboxTexture;
            
        public:
            virtual void initialize();
            virtual void draw(Camera& camera);
            SkyboxRenderpass(const std::string& shaderPath);

    };

}