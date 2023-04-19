#pragma once
#include "runtime/function/render/RenderPass.h"
#include "runtime/resource/res_type/components/Model.h"
#include <string>
#include <sys/types.h>
namespace EasyEngine {
    class GeometryRenderpass:public RenderPass{
    public:
        unsigned int gBuffer;
        unsigned int gPosition,gNormal,gAlbedoSpec;
        unsigned int gRbo;
    private:
        const int width,height;
        vector<Model> models;
        std::vector<glm::vec3> objectPositions;
    public:
        GeometryRenderpass(const std::string shaderPath);
        virtual void initialize();
        virtual void draw(Camera& camera);
        ~GeometryRenderpass();
    };
}