#include "PhongLightingRenderPass.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include <memory>
#include <string>


using namespace EasyEngine;
//To get visuals similar to Phong shading the specular shininess exponent has to be set a bit higher. 
//A general rule of thumb is to set it between 2 and 4 times the Phong shininess exponent. 
PhongLightingRenderPass::PhongLightingRenderPass(const string& shaderPath)
: RenderPass(shaderPath),objectColor(1.0f,0.5f,0.31f),ambientStrength(0.1f),specularStrength(1.0f){}

void PhongLightingRenderPass::initialize(){
    Model backpackModel(PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"backpack/backpack.obj"));
    models.emplace_back(backpackModel);
    shader->use();
    shader->setValue("material.ambient",1.0f,1.0f,1.0f);
    shader->setValue("material.diffuse",1.0f,1.0f,1.0f);
    shader->setValue("material.specular",0.5f, 0.5f, 0.5f);
    shader->setValue("material.shininess",32.0f);
    shader->setValue("directionalLight.ambient", 0.2f, 0.2f, 0.2f);
    shader->setValue("directionalLight.diffuse",0.5f, 0.5f, 0.5f);
    shader->setValue("directionalLight.specular",10.0f, 10.0f, 10.0f);
    shader->setValue("directionalLight.direction",-0.2f, -1.0f, -0.3f);
    glm::vec3 pointLightPositions[] = {
	glm::vec3( 0.7f,  0.2f,  2.0f),
	glm::vec3( 2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f, -3.0f)
    };  
    for(int i = 0;i<4;++i){
        shader->setValue("pointLights["+to_string(i)+"].position",pointLightPositions[i]);
        shader->setValue("pointLights["+to_string(i)+"].ambient", 0.05f, 0.05f, 0.05f);
        shader->setValue("pointLights["+to_string(i)+"].diffuse",0.8f, 0.8f, 0.8f);
        shader->setValue("pointLights["+to_string(i)+"].specular",1.0f, 1.0f, 1.0f);
        shader->setValue("pointLights["+to_string(i)+"].constant",1.0f);
        shader->setValue("pointLights["+to_string(i)+"].linear",0.09f);
        shader->setValue("pointLights["+to_string(i)+"].quadratic",0.032f);
        shader->setValue("pointLights["+to_string(i)+"].specular",1.0f, 1.0f, 1.0f);
    }
    shader->setValue("pointLights[0].diffuse",0.8f, 0.2f, 0.2f);
    shader->setValue("pointLights[1].diffuse",0.2f, 0.8f, 0.2f);
    shader->setValue("pointLights[2].diffuse",0.2f, 0.2f, 0.8f);
    shader->setValue("pointLights[3].diffuse",0.2f, 0.8f, 0.8f);
}


void PhongLightingRenderPass::draw(Camera& camera){

    shader->use();
    
    shader->setValue("spotLights[0].position",camera.cameraPosition);
    shader->setValue("spotLights[0].direction",camera.cameraFront);
    shader->setValue("spotLights[0].ambient", 0.05f, 0.05f, 0.05f);
    shader->setValue("spotLights[0].diffuse",0.8f, 0.8f, 0.8f);
    shader->setValue("spotLights[0].specular",1.0f, 1.0f, 1.0f);
    shader->setValue("spotLights[0].constant",1.0f);
    shader->setValue("spotLights[0].linear",0.09f);
    shader->setValue("spotLights[0].quadratic",0.032f);
    shader->setValue("spotLights[0].cutOff",glm::cos(glm::radians(15.0f)));
    shader->setValue("spotLights[0].outerCutOff",glm::cos(glm::radians(20.0f)));


    // shader->setValue("view",camera.getCameraPoseMatrix());
    // shader->setValue("projection",camera.getProjectionMatrix());
    shader->setValue("viewPos",camera.cameraPosition);


    model = glm::mat4(1.0f);
    shader->setValue("model",model);
    models[0].draw(*shader);
}
