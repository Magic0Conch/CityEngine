#pragma once
#include "render_pipeline.h"
#include "runtime/function/render/lighting/DiffuseLighting.h"
#include "runtime/resource/res_type/components/Camera.h"
#include <memory>
namespace EasyEngine {
    void RenderPipeline::uploadGlobalRenderResource(){

    }
    void RenderPipeline::initialize(){
        m_diffuse_light_pass = make_shared<DiffuseLighting>();
        m_diffuse_light_pass->initialize();
    }
    void RenderPipeline::forwardRender(Camera& camera){
        m_diffuse_light_pass->draw(camera);
    }
}