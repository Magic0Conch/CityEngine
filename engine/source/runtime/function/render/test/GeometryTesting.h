#pragma once
#include "runtime/function/render/RenderPass.h"
#include <sys/types.h>
namespace EasyEngine {
    class GeometryTesting: public RenderPass{
        public:
            unsigned int VAO;

        public:
            GeometryTesting(const std::string& vertShaderPath,const std::string& geomShaderPath,const std::string& fragShaderPath);
            virtual void initialize();
            virtual void draw(Camera& camera);
            virtual ~GeometryTesting();

    };
}
