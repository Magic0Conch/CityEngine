#pragma once
#include "glm/fwd.hpp"
#include "runtime/function/render/GeometryRenderpass.h"
#include "runtime/function/render/RenderPass.h"
#include "runtime/resource/res_type/components/Texture.h"
#include <memory>
#include <sys/types.h>
namespace EasyEngine {
    class PBRTesting:public RenderPass{
    private:
        int nrRows = 7;
        int nrColumns = 7;
        float spacing = 2.5;
        unsigned int captureFBO, captureRBO,envCubemap,irradianceMap;
    private:
        unsigned int textureHDR;
        shared_ptr<Texture> albedoMap;
        shared_ptr<Texture> normalMap;
        shared_ptr<Texture> metallicMap;
        shared_ptr<Texture> roughnessMap;
        shared_ptr<Texture> aoMap;

        shared_ptr<Shader> shaderEquirect;
        shared_ptr<Shader> shaderSkybox;
        shared_ptr<Shader> shaderIrradiance;

        vector<glm::vec3> lightPositions,lightColors;
        const int width,height;
    public:
        PBRTesting(const std::string shaderPath);
        virtual void initialize();
        virtual void draw(Camera& camera);
        ~PBRTesting();
    };
}