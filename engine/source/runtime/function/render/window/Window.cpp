#include "Window.h"
#include "runtime/function/render/window/Window.h"
#include <stdexcept>
namespace EasyEngine{
    std::unordered_map<GLFWwindow*, Window*> Window::__WINDOWS_MAP;
    Window::Window(const Device& p_device, const WindowSettings& p_windowSettings):
    m_windowSettings{p_windowSettings},
    m_device(p_device)
    {
    	createGlfwWindow(p_windowSettings);

        setCursorMode(p_windowSettings.cursorMode);
        setCursorShape(p_windowSettings.cursorShape);

        bindKeyCallback();
        bindMouseCallback();
        bindResizeCallback();
        bindCursorMoveCallback();

	    resizeEvent.addListener(std::bind(&Window::onResize, this, std::placeholders::_1, std::placeholders::_2));
	    cursorMoveEvent.addListener(std::bind(&Window::onMove, this, std::placeholders::_1, std::placeholders::_2));
    }
    Window::~Window(){
        glfwDestroyWindow(m_glfwWindow);
    }

    void Window::createGlfwWindow(const WindowSettings& p_windowSettings){
        
        m_windowSettings = p_windowSettings;
        glfwWindowHint(GLFW_SAMPLES,m_windowSettings.samples);
        m_glfwWindow = glfwCreateWindow(m_windowSettings.width,m_windowSettings.height,m_windowSettings.title.c_str() , nullptr, nullptr);
        if(m_glfwWindow == nullptr){
            throw std::runtime_error("Failed to create GLFW window");
        }
        else {
            int x, y;
	        glfwGetWindowPos(m_glfwWindow, &x, &y);
            m_cursorPosition.first = x;
            m_cursorPosition.second = y;
            __WINDOWS_MAP[m_glfwWindow] = this;
        }
    }

    // Callbacks binding 
    void Window::bindKeyCallback() const{
        auto keyCallback = [](GLFWwindow* p_window, int p_key, int p_scancode, int p_action, int p_mods){
            Window* windowInstance = findInstance(p_window);
            if (windowInstance){
                if (p_action == GLFW_PRESS)
                    windowInstance->keyPressedEvent.invoke(p_key);

                if (p_action == GLFW_RELEASE)
                    windowInstance->keyReleasedEvent.invoke(p_key);
            }
        };
        glfwSetKeyCallback(m_glfwWindow, keyCallback);
    }

    void Window::bindMouseCallback() const{
        auto mouseCallback = [](GLFWwindow* p_window,int p_button,int p_action,int p_mods){
            Window* windowInstance = findInstance(p_window);
            if(windowInstance){
                if(p_action == GLFW_PRESS)
                    windowInstance->mouseButtonPressedEvent.invoke(p_button);
                if(p_action == GLFW_RELEASE)
                    windowInstance->mouseButtonReleasedEvent.invoke(p_button);
            }
        };
        glfwSetMouseButtonCallback(m_glfwWindow, mouseCallback);
    }

    void Window::bindResizeCallback() const{
        auto resizeCallback=[](GLFWwindow* p_window,int p_width,int p_height){
            Window* windowInstance = findInstance(p_window);
            if(windowInstance){
                windowInstance->resizeEvent.invoke(p_width,p_height);
            }
        };
        glfwSetWindowSizeCallback(m_glfwWindow,resizeCallback);
    }

    void Window::bindCursorMoveCallback() const{
        auto cursorMoveCallback = [](GLFWwindow* p_window, double p_x, double p_y)
        {
            Window* windowInstance = findInstance(p_window);
            if (windowInstance){
                windowInstance->cursorMoveEvent.invoke(p_x,p_y);
            }
        };

	    glfwSetCursorPosCallback(m_glfwWindow, cursorMoveCallback);
    }

    void Window::setSize(uint16_t p_width, uint16_t p_height){

        glfwSetWindowSize(m_glfwWindow,p_width,p_height);
    }
    void Window::setShouldClose(bool p_value) const{
        glfwSetWindowShouldClose(m_glfwWindow, p_value);
    }
    bool Window::shouldClose() const{
        return glfwWindowShouldClose(m_glfwWindow);
    }
    bool Window::isFullscreen() const{
        return m_windowSettings.fullscreen;
    }
    void Window::makeCurrentContext() const{
        glfwMakeContextCurrent(m_glfwWindow);
    }

    void Window::swapBuffers() const{
	    glfwSwapBuffers(m_glfwWindow);
    }

    void Window::setCursorMode(Cursor::CursorMode p_cursorMode){
        m_windowSettings.cursorMode = p_cursorMode;
        glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, static_cast<int>(p_cursorMode));
    }
    void Window::setCursorShape(Cursor::CursorShape p_cursorShape){
        m_windowSettings.cursorShape = p_cursorShape;
        glfwSetCursor(m_glfwWindow, m_device.GetCursorInstance(p_cursorShape));
    }
    void Window::setCursorPosition(int16_t p_x, int16_t p_y){
        glfwSetCursorPos(m_glfwWindow, p_x, p_y);
    }
    void Window::setTitle(const std::string& p_title){
        m_windowSettings.title = p_title;
        glfwSetWindowTitle(m_glfwWindow, p_title.c_str());
    }
    GLFWwindow* Window::getGlfwWindow() const{
        return m_glfwWindow;
    }

    //Event Listeners
    void Window::onResize(uint16_t p_width, uint16_t p_height){
        m_windowSettings.width = p_width;
        m_windowSettings.height = p_height;
    }
    void Window::onMove(int16_t p_x, int16_t p_y){
        m_cursorPosition.first = p_x;
        m_cursorPosition.second = p_y;
    }

    Window* Window::findInstance(GLFWwindow* p_glfwWindow){
        return __WINDOWS_MAP.find(p_glfwWindow) != __WINDOWS_MAP.end()?__WINDOWS_MAP[p_glfwWindow]:nullptr;
    }
}
