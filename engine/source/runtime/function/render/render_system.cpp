#include "render_system.h"
#include "glm/fwd.hpp"
#include "runtime/core/base/macro.h"
#include "runtime/function/framework/world/world_manager.h"
#include "runtime/include/CameraController.h"
#include <memory>

namespace EasyEngine {

    void RenderSystem::initialize(){
        WorldManager::getInstance().initialize();
        LogInfo("load all models!");

        // setup render camera
        m_render_camera->setCameraPosition(glm::vec3(0,0,3));
        m_render_camera = std::make_shared<Camera>();
        m_render_camera->farPlane = 1000.f;

        m_camera_controller = make_shared<CameraController>(*m_render_camera);

        m_render_pipeline = make_shared<RenderPipeline>();
        m_render_pipeline->initialize();

        //set ubo
        glGenBuffers(1,&uboMatrices);
        glBindBuffer(GL_UNIFORM_BUFFER,uboMatrices);
        glBufferData(GL_UNIFORM_BUFFER,2*sizeof(glm::mat4)+sizeof(glm::vec3),NULL,GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER,0);
        glBindBufferRange(GL_UNIFORM_BUFFER,0,uboMatrices,0,2*sizeof(glm::mat4)+sizeof(glm::vec3));
        
    }
    
    void RenderSystem::tick(){

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

        //update camera matrix perframe        
        glBindBuffer(GL_UNIFORM_BUFFER,uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::mat4),glm::value_ptr(m_render_camera->getProjectionMatrix()));
        glBufferSubData(GL_UNIFORM_BUFFER,sizeof(glm::mat4),sizeof(glm::mat4),glm::value_ptr(m_render_camera->getCameraPoseMatrix()));
        glBufferSubData(GL_UNIFORM_BUFFER,2*sizeof(glm::mat4),sizeof(glm::vec3),glm::value_ptr(m_render_camera->cameraPosition));
        glBindBuffer(GL_UNIFORM_BUFFER,0);

        //TODO pipeline tick 暂时都是forward rendering，等写好Render Pipeline
        m_render_pipeline->forwardRender(*m_render_camera);
        m_camera_controller->processInput();
    }

    //TODO 这里得写好tickLogic才能知道要交换什么数据
    void RenderSystem::swapLogicRenderData(){
        
    }
}