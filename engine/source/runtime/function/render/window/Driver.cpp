#include "Driver.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <iostream>
namespace EasyEngine{
    Driver::Driver(){
        initGlad();
        m_isActive=true;
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glCullFace(GL_BACK);
    }

    bool Driver::isActive() const{
        return m_isActive;
    }

    void Driver::initGlad(){
        const GLenum success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (!success) {
            std::cout << "GLAD initializaion failed!" << std::endl;
        }    
    }

}