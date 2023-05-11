#include "Engine.h"
#include "GLFW/glfw3.h"
#include "runtime/Engine.h"
#include "runtime/core/base/macro.h"
#include "runtime/function/render/lighting/DiffuseLighting.h"
#include "runtime/function/render/lighting/PhongLightingRenderPass.h"
#include "runtime/function/render/preprocessing/PreProcessingPass.h"
#include "runtime/include/CameraController.h"
#include "runtime/include/EngineWindow.h"
#include "runtime/include/InputHandler.h"
#include "runtime/include/WindowTime.h"
#include <iostream>
#include <memory>
#include <string>
#include "platform/path_utility/PathUtility.h"
using namespace EasyEngine;

namespace EasyEngine {
    int drawcall;

    Engine::Engine(){
        printf("Binary Root: %s\n", BINARY_ROOT_DIR);
        const std::string configPath = PU::getFullPath(BINARY_ROOT_DIR, "configs\\global_config.ini");
        g_global_context.initializeContext(configPath);	
        // cameraController = make_shared<CameraController>(*camera);
    }



    void Engine::start(){

        EngineWindow::getInstance().createWindow();
        //load the address of the OpenGL pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            cout << "GLAD initializaion failed!" << endl;
        }    
        m_renderSystem = make_shared<RenderSystem>();
        initialize();
        LogInfo("Engine initialize success!");
        glEnable(GL_DEPTH_TEST);
        // glEnable(GL_CULL_FACE);

        //Main render loop
        while (!glfwWindowShouldClose(EngineWindow::getInstance().window) && !InputHandler::getInstance().closeWindow) {		
            mainLoop();
        }
        glfwTerminate();
    }

    void Engine::initialize(){
        m_renderSystem->initialize(); 
    }

    void Engine::mainLoop() {
        //Time（应该放在逻辑里）
        WindowTime::updateTimeValue();
        drawcall = 0;
        calculateFPS(WindowTime::deltaTimeValue);
        LogInfo("FPS:"+to_string(m_fps)+"\n");
        
        //Events 也放在逻辑里
        glfwPollEvents();
        InputHandler::getInstance().handleInput(EngineWindow::getInstance().window);

        //render
        tickRender();

        // cout<<drawcall<<endl;
        glfwSwapBuffers(EngineWindow::getInstance().window);
    }

    void Engine::tickRender() {
        m_renderSystem->tick();
    }

    void Engine::calculateFPS(float delta_time){
        ++m_frame_count;
        if(m_frame_count==1){
            m_average_duration = delta_time;
        }
        else{
            m_average_duration = (m_average_duration * (m_frame_count-1) + delta_time)/m_frame_count;
        }
        // m_fps = 1.0f/m_average_duration;
        m_fps = 1.0f/delta_time;
    }

}
int main(int argc,char** argv) {
    EasyEngine::Engine engine;    
    engine.start();
    return 0;
}