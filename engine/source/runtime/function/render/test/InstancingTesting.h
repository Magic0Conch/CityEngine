#pragma once
#include "glm/fwd.hpp"
#include "runtime/function/render/RenderPass.h"
#include <sys/types.h>
#include <vector>
namespace EasyEngine {
    class InstancingTesting: public RenderPass{
        private:
            std::vector<glm::vec2> translations;
        public:
            unsigned int VAO;

        public:
            InstancingTesting(const std::string& shaderPath);
            virtual void initialize();
            virtual void draw(Camera& camera);
            virtual ~InstancingTesting();

    };
}
