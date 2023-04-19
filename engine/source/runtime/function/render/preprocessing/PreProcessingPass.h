#pragma once
#include "runtime/function/render/RenderPass.h"
#include "runtime/resource/res_type/components/Camera.h"
#include <memory>
#include <sys/types.h>
namespace EasyEngine {
    class PreProcessingPass:public RenderPass{
        private:
            
        public:
            unsigned int uboMatrices;
            virtual void initialize();
            virtual void draw(Camera& camera);
            PreProcessingPass();

    };

}