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

    class PhongLightingRenderPass:public RenderPass{
    private:
        glm::vec3 resultColor;
        glm::vec3 objectColor;

        float ambientStrength;
        float specularStrength;

        shared_ptr<Texture> diffuseMap;
        shared_ptr<Texture> specularMap;
        vector<Model> models;
    public:
        PhongLightingRenderPass(const string& shaderPath);
        virtual void draw(Camera& camera);
        // virtual void draw(const glm::mat4& viewMatrix,const glm::mat4& projectionMatrix,const glm::vec3& viewPos);
        virtual void initialize();
    };
}