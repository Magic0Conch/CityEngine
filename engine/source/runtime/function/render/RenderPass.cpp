#include "RenderPass.h"
#include <memory>


using namespace EasyEngine;


RenderPass::RenderPass(const std::string& shaderPath)
{
    shader = std::make_shared<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), shaderPath+".vert").c_str()
               ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), shaderPath+".frag").c_str());
    uniformBlockIndex = glGetUniformBlockIndex(shader->ID,"Matrices");
    glUniformBlockBinding(shader->ID,uniformBlockIndex,0);
}

RenderPass::RenderPass(const std::string& vertpath,const std::string& geompath,const std::string& fragpath){
    shader = std::make_shared<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), vertpath+".vert").c_str()
    ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), geompath+".geom").c_str()
    ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), fragpath+".frag").c_str()
    );
    uniformBlockIndex = glGetUniformBlockIndex(shader->ID,"Matrices");
    glUniformBlockBinding(shader->ID,uniformBlockIndex,0);
}

RenderPass::RenderPass(){

}

RenderPass::RenderPass(const std::string& vertpath,const std::string& fragpath){
    shader = std::make_shared<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), vertpath+".vert").c_str()
            ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), fragpath+".frag").c_str());
    uniformBlockIndex = glGetUniformBlockIndex(shader->ID,"Matrices");
    glUniformBlockBinding(shader->ID,uniformBlockIndex,0);
}