#pragma once
#include <iostream>
#include <string.h>
#include <string>
#include "runtime/include/Shader.h"
namespace EasyEngine {
    class PostProcessingParameter{
    public:
        std::string parameterName;
        virtual void setShaderParameter(Shader& shader)=0;
        PostProcessingParameter(const std::string& name);
    };

    class PostProcessingParameterFloat:public PostProcessingParameter{
    public:
        float value;
        virtual void setShaderParameter(Shader& shader);
        PostProcessingParameterFloat(const std::string& name,float val);
    };

    class PostProcessingParameterInt:public PostProcessingParameter{
    public:
        int value;
        virtual void setShaderParameter(Shader& shader);
        PostProcessingParameterInt(const std::string& name,int val);
    };

    class PostProcessingParameterUInt:public PostProcessingParameter{
    public:
        unsigned int value;
        virtual void setShaderParameter(Shader& shader);
        PostProcessingParameterUInt(const std::string& name,unsigned int val);
    };
}
