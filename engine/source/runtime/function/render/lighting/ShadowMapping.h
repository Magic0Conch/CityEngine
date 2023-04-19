#pragma once
#include <glad/glad.h>
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include "runtime/resource/res_type/components/Model.h"
#include "../../global/global_context.h"
#include "../RenderPass.h"
namespace EasyEngine {
    using PU = EasyEngine::PathUtility;

    class ShadowMapping:public RenderPass{
    private:
        unsigned int planeVAO, planeVBO;
        shared_ptr<Texture> floorTexture;
        shared_ptr<Shader> shaderDepth;
        const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
        unsigned int cubeVAO = 0;
        unsigned int cubeVBO = 0;
        unsigned int depthMap;
        shared_ptr<Texture> diffuseMap;
        shared_ptr<Texture> specularMap;
        vector<Model> models;
        shared_ptr<Camera> lightCamera;

        void drawScene(Shader& curShader);
        void drawCube();
    public:

        unsigned int depthMapFBO;   
        ShadowMapping(const string& shaderPath);
        virtual void draw(Camera& camera);
        // virtual void draw(const glm::mat4& viewMatrix,const glm::mat4& projectionMatrix,const glm::vec3& viewPos);
        virtual void initialize();
    };
}