#pragma once
#include "glm/fwd.hpp"
#include "runtime/function/render/RenderPass.h"
#include "runtime/resource/res_type/components/Model.h"
#include <memory>
#include <sys/types.h>
#include <vector>
namespace EasyEngine {
    class AsteroidField: public RenderPass{
        private:
            const unsigned int amount = 100000;
            std::vector<glm::mat4> rockModels;
            std::shared_ptr<Model> rockModel, planetModel;
        public:
            unsigned int VAO;

        public:
            AsteroidField(const std::string& shaderPath);
            virtual void initialize();
            virtual void draw(Camera& camera);
            virtual ~AsteroidField();

    };
}
