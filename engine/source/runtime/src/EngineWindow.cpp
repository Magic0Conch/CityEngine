#include "../include/EngineWindow.h"
#include "GLFW/glfw3.h"
#include <cstdio>

using namespace EasyEngine;

EngineWindow::EngineWindow(){
    viewportHeight = 600;
    viewportWidth = 800;
}

EngineWindow::EngineWindow(float viewportWidth,float viewportHeight):viewportHeight(viewportHeight),viewportWidth(viewportWidth){

}

EngineWindow& EngineWindow::getInstance(){
    static EngineWindow instance;
    return instance;
}

void EngineWindow::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    EngineWindow::getInstance().viewportWidth = width;
    EngineWindow::getInstance().viewportHeight = height;
    glViewport(0, 0, width, height);
}

void EngineWindow::createWindow(){
    if (!glfwInit()) {
        const char* msg;
        glfwGetError(&msg);
        printf("Error: %s \n", msg);
    }
    
    
    //configure the options prefixed with GLFW_
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //anti aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);
    // glEnable(GL_MULTISAMPLE);
    
    //create the window
    window = glfwCreateWindow(EngineWindow::getInstance().viewportWidth, EngineWindow::getInstance().viewportHeight,  "Rasterization", NULL, NULL);//window witdth,height,name
    if (window == NULL) {
        std::cout << "create window failed" << std::endl;
        glfwTerminate();
        return;
    }
    //tell GLFW to make the context of our window the main context on the current threaed
    glfwMakeContextCurrent(window);

    // glViewport(0, 0, EngineWindow::getInstance().viewportWidth, EngineWindow::getInstance().viewportHeight);//position, size(in pixels)
    // glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}