#pragma once
#include "glm/fwd.hpp"
#include "runtime/include/Shader.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>
#include "../global/global_context.h"
#include "../../resource/res_type/components/Camera.h"
#include "../../include/JsonManipulation.h"
#include "runtime/resource/res_type/common/PostProcessingParameter.h"
namespace EasyEngine {
    using PU = EasyEngine::PathUtility;
    extern GlobalContext g_global_context;
    

    class RenderPass{
    public:
        Json::Value sceneConfig;
        glm::mat4 model = glm::mat4(1.0);
        std::shared_ptr<Shader> shader;
        unsigned int uniformBlockIndex;
        std::vector<std::shared_ptr<PostProcessingParameter> > parameters;
    public:
        virtual void initialize()=0;
        virtual void draw(Camera& camera)=0;
        RenderPass(const std::string& shaderPath);
        RenderPass(const std::string& vertpath,const std::string& fragpath);
        RenderPass(const std::string& vertpath,const std::string& geompath,const std::string& fragpath);
        RenderPass();
    };

}