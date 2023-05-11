#pragma once
#include <stdint.h>
#include <string>
#include "../Cursor.h"
namespace EasyEngine {
    struct WindowSettings{
        std::string title = "default";
        uint16_t width = 1200;
        uint16_t height = 800;
        bool fullscreen = false;

        uint32_t samples = 4;
		Cursor::CursorMode cursorMode = Cursor::CursorMode::NORMAL;		
		Cursor::CursorShape cursorShape = Cursor::CursorShape::ARROW;

    };

}