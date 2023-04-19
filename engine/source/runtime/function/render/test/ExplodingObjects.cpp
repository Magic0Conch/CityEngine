#include "ExplodingObjects.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "runtime/include/WindowTime.h"
#include <memory>
#include <string>


using namespace EasyEngine;

ExplodingObjects::ExplodingObjects(const string& shaderPath)
: RenderPass(shaderPath,shaderPath,shaderPath){}

void ExplodingObjects::initialize(){
    Model backpackModel(PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"backpack/backpack.obj"));
    models.emplace_back(backpackModel);
    shader->use();
}


void ExplodingObjects::draw(Camera& camera){
    
    shader->use();
    // shader->setValue("time",(float)WindowTime::currentTimeValue);
    model = glm::mat4(1.0f);
    shader->setValue("model",glm::translate(glm::mat4(1.0), glm::vec3(0,0,-2)));
    models[0].draw(*shader);
    
}
