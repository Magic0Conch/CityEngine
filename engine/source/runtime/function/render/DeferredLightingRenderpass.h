#pragma once
#include "glm/fwd.hpp"
#include "runtime/function/render/GeometryRenderpass.h"
#include "runtime/function/render/RenderPass.h"
#include <sys/types.h>
namespace EasyEngine {
    class DeferredLightingRenderpass:public RenderPass{
    private:
        unsigned int gBuffer;
        unsigned int gPosition,gNormal,gAlbedoSpec;
        unsigned int gRbo;
        shared_ptr<GeometryRenderpass> grsp;
        shared_ptr<Shader> shaderLightBox;
        vector<glm::vec3> lightPositions,lightColors;
        const int width,height;

        unsigned int quadVAO,quadVBO;
        unsigned int cubeVAO,cubeVBO;
        void renderQuad();
        void renderCube();
    public:
        DeferredLightingRenderpass(const std::string vertPath,const std::string fragPath,shared_ptr<GeometryRenderpass> grsp);
        virtual void initialize();
        virtual void draw(Camera& camera);
        ~DeferredLightingRenderpass();
    };
}