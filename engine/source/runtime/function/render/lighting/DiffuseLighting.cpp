#include "DiffuseLighting.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "runtime/function/render/lighting/DiffuseLighting.h"
#include <memory>
#include <string>


using namespace EasyEngine;

DiffuseLighting::DiffuseLighting(const vector<shared_ptr<Entity>>& entities_list)
: RenderPass("lighting/diffuse"),ambientStrength(0.1f),entites{entities_list}{

}

void DiffuseLighting::initialize(){
    shader->use();
    shader->setValue("material.ambient",1.0f,1.0f,1.0f);
    shader->setValue("material.diffuse",1.0f,1.0f,1.0f);
    shader->setValue("material.specular",0.5f, 0.5f, 0.5f);
    shader->setValue("material.shininess",32.0f);
    shader->setValue("directionalLight.ambient", 1.0f, 1.0f, 1.0f);
    shader->setValue("directionalLight.diffuse",0.0f, 0.0f, 0.0f);
    shader->setValue("directionalLight.specular",10.0f, 10.0f, 10.0f);
    shader->setValue("directionalLight.direction",-0.2f, -1.0f, -0.3f);
}


void DiffuseLighting::draw(Camera& camera){
    shader->use();
    shader->setValue("viewPos",camera.cameraPosition);
    model = glm::mat4(1.0f);
    shader->setValue("model",model);
    for(auto entity:entites){
        entity->model->draw(*shader);
    }
}
