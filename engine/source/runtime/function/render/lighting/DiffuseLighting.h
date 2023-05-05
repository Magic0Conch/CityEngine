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
#include "runtime/function/framework/entity/entity.h"


namespace EasyEngine {
    using PU = EasyEngine::PathUtility;
    class DiffuseLighting:public RenderPass{
    private:
        float ambientStrength;
        vector<shared_ptr<Entity>> entites;
    public:
        DiffuseLighting() = delete;
        DiffuseLighting(const vector<shared_ptr<Entity>>&);
        virtual void draw(Camera& camera);
        virtual void initialize();
    };
}