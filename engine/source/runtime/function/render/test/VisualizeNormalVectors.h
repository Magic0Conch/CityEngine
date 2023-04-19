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
    class VisualizeNormalVectors:public RenderPass{
    private:
        vector<Model> models;
        shared_ptr<Shader> normalDisplayShader;
    public:
        VisualizeNormalVectors(const string& shaderPath,const string& normalDisplayShaderPath);
        virtual void draw(Camera& camera);
        // virtual void draw(const glm::mat4& viewMatrix,const glm::mat4& projectionMatrix,const glm::vec3& viewPos);
        virtual void initialize();
    };
}