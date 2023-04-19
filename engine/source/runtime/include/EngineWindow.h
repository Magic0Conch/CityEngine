#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
namespace EasyEngine {
    class EngineWindow{
    private:
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    public:
        float viewportWidth;
        float viewportHeight;
        GLFWwindow* window;
    public:
        EngineWindow();
        EngineWindow(float viewportWidth,float viewportHeight);
        static EngineWindow& getInstance();
        void createWindow();
    };

}