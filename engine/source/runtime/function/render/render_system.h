#pragma once
#include "runtime/include/CameraController.h"
#include "runtime/resource/res_type/components/Camera.h"
#include "render_pipeline.h"
#include <memory>
namespace EasyEngine {
    class RenderSystem{
        public:
            RenderSystem() = default;
            void initialize();
            void tick();
            // void clear() = delete;
            void swapLogicRenderData();
        public:
            std::shared_ptr<RenderPipeline> m_render_pipeline;
            std::shared_ptr<Camera> m_render_camera;
            shared_ptr<CameraController> m_camera_controller;   
            //getGuidOfPickedMesh getGObjectIDByMeshID
            unsigned int uboMatrices;
    };
}