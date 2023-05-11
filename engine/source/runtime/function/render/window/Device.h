#pragma once
#include "GLFW/glfw3.h"
#include "runtime/function/render/window/Cursor.h"
#include "settings/DeviceSettings.h"
#include "Cursor.h"
#include <unordered_map>
namespace EasyEngine {
    class Device{
    public:
        Device(const DeviceSettings& p_deviceSettings);
        ~Device();
        GLFWcursor* GetCursorInstance(Cursor::CursorShape p_cursorShape) const;
        void pollEvents() const;
    private:
        void createCursors();
        void destoryCursors();

        bool m_isAlive = false;
		std::unordered_map<Cursor::CursorShape, GLFWcursor*> m_cursors;
    };
}