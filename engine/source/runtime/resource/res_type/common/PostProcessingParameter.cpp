#include "PostProcessingParameter.h"
#include "runtime/function/render/RenderPass.h"
#include <string>
#include <sys/types.h>

using namespace EasyEngine;

PostProcessingParameter::PostProcessingParameter(const std::string& name):parameterName(name){}


PostProcessingParameterFloat::PostProcessingParameterFloat(const std::string& name,float val):PostProcessingParameter(name),value(val){}
void PostProcessingParameterFloat::setShaderParameter(Shader& shader){
    shader.setValue(parameterName,value);
}



PostProcessingParameterInt::PostProcessingParameterInt(const std::string& name,int val):PostProcessingParameter(name),value(val){}
void PostProcessingParameterInt::setShaderParameter(Shader& shader){
    shader.setValue(parameterName,value);
}

PostProcessingParameterUInt::PostProcessingParameterUInt(const std::string& name,unsigned int val):PostProcessingParameter(name),value(val){}
void PostProcessingParameterUInt::setShaderParameter(Shader& shader){
    shader.setValue(parameterName,(int)value);
}