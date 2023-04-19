#pragma once
#include "runtime/function/render/GeometryRenderpass.h"
#include "runtime/resource/res_type/components/Model.h"
#include <memory>
#include <string>
#include <sys/types.h>
#include <vector>
namespace EasyEngine {
    class SSAOTest:public RenderPass{
    public:
        unsigned int gBuffer;
        unsigned int gPosition,gNormal,gAlbedoSpec;
        unsigned int gRbo;
        unsigned int noiseTexture;
        unsigned int ssaoFBO,ssaoColorBuffer;
        unsigned int ssaoBlurFBO,ssaoColorBufferBlur;
    private:
        const int width,height;
        vector<Model> models;
        std::vector<glm::vec3> ssaoKernels,ssaoNoises;
        std::vector<glm::vec3> lightPositions,lightColors;
        shared_ptr<Shader> shaderBlur;
        shared_ptr<Shader> shaderLighting;

        shared_ptr<GeometryRenderpass> grsp_view;
        void generateSsaoParameters();
    public:
        SSAOTest(const std::string vertPath,const std::string fragPath,shared_ptr<GeometryRenderpass> grsp_view);
        virtual void initialize();
        virtual void draw(Camera& camera);
        ~SSAOTest();
    };
}