#include "Engine.h"
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
    auto getAllPathWithExt(const std::string&, const std::string&);
    Engine::Engine():worldManager(WorldManager::getInstance()){
        printf("Binary Root: %s\n", BINARY_ROOT_DIR);
        const std::string configPath = PU::getFullPath(BINARY_ROOT_DIR, "configs\\global_config.ini");
        g_global_context.initializeContext(configPath);	
        cameraController = make_shared<CameraController>(*camera);
    }



    void Engine::start(){

        EngineWindow::getInstance().createWindow();
        //load the address of the OpenGL pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            cout << "GLAD initializaion failed!" << endl;
        }    
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
        
        worldManager.initialize();
        LogInfo("load all models!\n");
        renderPasses.emplace_back(make_shared<PreProcessingPass>());
        // renderPasses.emplace_back(make_shared<PhongLightingRenderPass>("lighting/phong"));
        renderPasses.emplace_back(make_shared<DiffuseLighting>(worldManager.entity_list));

        for (const shared_ptr<RenderPass> rp : renderPasses) {
            rp->initialize();
        }    
    }

    void Engine::mainLoop() {
        //Time
        WindowTime::updateTimeValue();
        drawcall = 0;
        calculateFPS(WindowTime::deltaTimeValue);
        LogInfo("FPS:"+to_string(m_fps)+"\n");
        
        //Events
        glfwPollEvents();
        InputHandler::getInstance().handleInput(EngineWindow::getInstance().window);
        cameraController->processInput();
        //render
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
        tickRender();
        cout<<drawcall<<endl;
        glfwSwapBuffers(EngineWindow::getInstance().window);
    }

    void Engine::tickRender() {
        

        projection = camera->getProjectionMatrix();
        view = camera->getCameraPoseMatrix();
        // colorsRenderPass->draw(camera);
        for (const shared_ptr<RenderPass> rp : renderPasses) {
            rp->draw(*camera);
        }    
        // phongLightingRenderPass->draw(camera);
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
    EasyEngine::PathUtility::getAllPathWithExt("C:\\Data\\snow3\\Productions\\Production_2\\Data",".obj");
    engine.start();
    return 0;
}