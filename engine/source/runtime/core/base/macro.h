#pragma once
// #include <GL/gl.h>
#include "runtime/core/base/macro.h"
#include <glad/glad.h>
#include <iostream>
#include <string>

#if defined (_WIN32)
    #define PLATFORM_NAME "windows"
#elif defined(_WIN64)
    #define PLATFORM_NAME "windows"
#elif defined(__linux__)
    #define PLATFORM_NAME "linux"
#else
    #define PLARFORM_NAME NULL
#endif


inline GLenum glCheckError_(const char *file,int line){
    GLenum errorCode;
    while ((errorCode = glGetError())!=GL_NO_ERROR) {
        std::string errorMsg;
        switch (errorCode) {
            case GL_INVALID_ENUM:                  errorMsg = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 errorMsg = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             errorMsg = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                errorMsg = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               errorMsg = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 errorMsg = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: errorMsg = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout<<errorMsg<<": occured in "<<file<<"line:("<<line<<")."<<std::endl;
    }
    return errorCode;
}

enum class PlatformNames{
    invalid,windows,linux
};

inline PlatformNames getPlatformName_(){
    
    auto platformName = static_cast<std:: string>(PLATFORM_NAME);
    if(platformName=="windows"){
        return PlatformNames::windows;
    }
    else if (platformName=="linux") {
        return PlatformNames::linux;
    }    
    return PlatformNames::invalid;
}

inline void LogInfo(std::string msg){
    std::cout<<msg;
}

#define glCheckError() glCheckError_(__FILE__, __LINE__)
#define getPlatformName() getPlatformName_()