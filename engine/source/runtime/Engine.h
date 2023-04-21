#pragma once
#include "include/Utility.h"
#include "include/Shader.h"
#include "include/EngineWindow.h"
#include "include/ImageProcessing.h"
#include "include/WindowTime.h"
#include "function/framework/world/world_manager.h"
#include "include/CameraController.h"
#include "include/JsonManipulation.h"
#include "function/global/global_context.h"
#include "function/render/render_system.h"
#include <iostream>
#include <memory>
#include <string>
#include <runtime/core/base/macro.h>
#include "platform/path_utility/PathUtility.h"

#ifdef BINARY_ROOT_DIR
// #define BINARY_ROOT_DIR_C BINARY_ROOT_DIR
#endif
using PU = EasyEngine::PathUtility;

class RenderPass;
namespace EasyEngine {

	using namespace std;
    class Engine{
    private:
        // shared_ptr<Camera> camera = make_shared<Camera>();
        // shared_ptr<CameraController> cameraController;   
        // vector<shared_ptr<RenderPass>> renderPasses;
        // WorldManager& worldManager;

        // glm::mat4 view = glm::mat4(1.0f);
        // glm::mat4 projection;
        std::string binaryRootDir;
        shared_ptr<RenderSystem> m_render_system;
    public:
        Engine();       
        void getEnviromentInfomation();
        void initialize();
        void tickRender();
        void mainLoop();
        void start();

        void clear();
        bool isQuit() const;

    protected:
        bool m_is_quit{false};
        float m_average_duration{0.f};
        int m_frame_count{0};
        int m_fps{0};
        
        void calculateFPS(float delta_time);
    };
}