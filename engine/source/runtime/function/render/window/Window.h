#pragma once
#include "GLFW/glfw3.h"
#include "runtime/function/render/window/Device.h"
#include "runtime/function/render/window/settings/WindowSettings.h"
// #include <tool/event/Event.h>
// #include <event/Event.h>
#include <tool/event/Event.h>
#include <memory>
#include <stdint.h>
#include <utility>
// #include "settings/DeviceSettings.h"
// #include <string>
namespace EasyEngine {
    class Window{
    public:
        //Input events
        Tool::Event<int> keyPressedEvent;
        Tool::Event<int> keyReleasedEvent;
        Tool::Event<int> mouseButtonPressedEvent;
        Tool::Event<int> mouseButtonReleasedEvent;

        //Window events
        EasyEngine::Tool::Event<uint16_t,uint16_t> resizeEvent;
        Tool::Event<int16_t,int16_t> cursorMoveEvent;

        Window(const Device& p_device, const WindowSettings& p_windowSettings);
        ~Window();

        static Window* findInstance(GLFWwindow* p_glfwWindow);
		GLFWwindow* getGlfwWindow() const;

    private:
        void createGlfwWindow(const WindowSettings& p_windowSettings);
        // Callbacks binding 
		void bindKeyCallback() const;
		void bindMouseCallback() const;
		void bindResizeCallback() const;
        void bindCursorMoveCallback() const;

		void setSize(uint16_t p_width, uint16_t p_height);
        void setShouldClose(bool p_value) const;
        bool shouldClose() const;
        bool isFullscreen() const;
		void makeCurrentContext() const;

		void swapBuffers() const;

		void setCursorMode(Cursor::CursorMode p_cursorMode);
		void setCursorShape(Cursor::CursorShape p_cursorShape);
		void setCursorPosition(int16_t p_x, int16_t p_y);
		void setTitle(const std::string& p_title);
        

        //Event Listeners
        void onResize(uint16_t p_width, uint16_t p_height);
        void onMove(int16_t p_x, int16_t p_y);

    private:
        const Device& m_device;
		GLFWwindow* m_glfwWindow;
        WindowSettings m_windowSettings;
        std::pair<int16_t, int16_t> m_cursorPosition;
        static std::unordered_map<GLFWwindow*,Window*> __WINDOWS_MAP;
    };
}