
#include "VisualizeNormalVectors.h"
#include <sys/types.h>


using namespace EasyEngine;

VisualizeNormalVectors::VisualizeNormalVectors(const string& shaderPath,const string& normalDisplayShaderPath)
: RenderPass(shaderPath,shaderPath,shaderPath){
    normalDisplayShader =  std::make_shared<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), normalDisplayShaderPath+".vert").c_str()
    ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), normalDisplayShaderPath+".geom").c_str()
    ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), normalDisplayShaderPath+".frag").c_str()
    );

    unsigned int blockIndex = glGetUniformBlockIndex(normalDisplayShader->ID,"Matrices");
    glUniformBlockBinding(normalDisplayShader->ID,blockIndex,0);
}

void VisualizeNormalVectors::initialize(){
    Model backpackModel(PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"backpack/backpack.obj"));
    models.emplace_back(backpackModel);
    shader->use();
}


void VisualizeNormalVectors::draw(Camera& camera){
    
    model = glm::mat4(1.0f);
    model = glm::translate(glm::mat4(1.0), glm::vec3(0,0,-2));
    shader->use();
    shader->setValue("model",model);
    models[0].draw(*shader);

    normalDisplayShader->use();
    normalDisplayShader->setValue("model",model);
    models[0].draw(*normalDisplayShader);
}
