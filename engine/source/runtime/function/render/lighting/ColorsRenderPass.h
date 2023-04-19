#pragma once
#include <glad/glad.h>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <string>
#include "runtime/resource/res_type/components/Model.h"
#include "../../../include/Shader.h"
#include "../../global/global_context.h"
#include "../RenderPass.h"
namespace EasyEngine {
    using PU = EasyEngine::PathUtility;

    class ColorsRenderPass:public RenderPass{
    private:        
        glm::vec3 objectColor;

    private:
        void setModelMatrix(const glm::mat4& modelMatrix);
        void setViewMatrix(const glm::mat4& view);
        void setProjectionMatrix(const glm::mat4& projection);
    public:
        ColorsRenderPass(const string& shaderPath);

        virtual void draw(Camera& camera);
        virtual void initialize();
    };
}