#pragma once
#include "glm/fwd.hpp"
#include "runtime/function/render/RenderPass.h"
#include "runtime/resource/res_type/components/Model.h"
#include "runtime/resource/res_type/components/Texture.h"
#include <memory>
#include <sys/types.h>
#include <vector>
namespace EasyEngine {
    class GammaCorrectionTesting: public RenderPass{
        private:
        public:
            const int samples = 4;
            unsigned int planeVAO, planeVBO;
            shared_ptr<Texture> floorTexture;
            shared_ptr<Texture> floorTextureGammaCorrected;
        public:
            bool gammaEnabled = false;
            GammaCorrectionTesting(const std::string& shaderPath);
            virtual void initialize();
            virtual void draw(Camera& camera);
            virtual ~GammaCorrectionTesting();

    };
}
