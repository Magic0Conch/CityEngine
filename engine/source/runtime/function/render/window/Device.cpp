#include "Device.h"
#include "GLFW/glfw3.h"
#include "runtime/function/render/window/Device.h"
#include <stdexcept>

using namespace EasyEngine;

Device::Device(const DeviceSettings& p_deviceSettings){
    if(glfwInit()==GLFW_FALSE){
        throw std::runtime_error("Init glfw failed!");
        glfwTerminate();
    }
    else {
        createCursors();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, p_deviceSettings.contextVersionMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, p_deviceSettings.contextVersionMinor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES,p_deviceSettings.samples);

        m_isAlive = true;
    }
}

Device::~Device(){
    if(m_isAlive){
        destoryCursors();
        glfwTerminate();
    }
}


void Device::pollEvents() const{
    glfwPollEvents();
}

void Device::createCursors(){
    m_cursors[Cursor::CursorShape::ARROW] = glfwCreateStandardCursor(static_cast<int>(Cursor::CursorShape::ARROW));
    m_cursors[Cursor::CursorShape::IBEAM] = glfwCreateStandardCursor(static_cast<int>(Cursor::CursorShape::IBEAM));
	m_cursors[Cursor::CursorShape::CROSSHAIR] = glfwCreateStandardCursor(static_cast<int>(Cursor::CursorShape::CROSSHAIR));
	m_cursors[Cursor::CursorShape::HAND] = glfwCreateStandardCursor(static_cast<int>(Cursor::CursorShape::HAND));
	m_cursors[Cursor::CursorShape::HRESIZE] = glfwCreateStandardCursor(static_cast<int>(Cursor::CursorShape::HRESIZE));
	m_cursors[Cursor::CursorShape::VRESIZE] = glfwCreateStandardCursor(static_cast<int>(Cursor::CursorShape::VRESIZE));
}

void Device::destoryCursors(){
    glfwDestroyCursor(m_cursors[Cursor::CursorShape::ARROW]);
	glfwDestroyCursor(m_cursors[Cursor::CursorShape::IBEAM]);
	glfwDestroyCursor(m_cursors[Cursor::CursorShape::CROSSHAIR]);
	glfwDestroyCursor(m_cursors[Cursor::CursorShape::HAND]);
	glfwDestroyCursor(m_cursors[Cursor::CursorShape::HRESIZE]);
	glfwDestroyCursor(m_cursors[Cursor::CursorShape::VRESIZE]);
}

GLFWcursor* Device::GetCursorInstance(Cursor::CursorShape p_cursorShape) const{
    return m_cursors.at(p_cursorShape);
}