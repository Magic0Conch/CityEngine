#pragma once
#include <GLFW/glfw3.h>
namespace EasyEngine {
	class WindowTime
	{
	public:
		static double deltaTimeValue;
		static double lastTimeValue;
		static double currentTimeValue;
		static int frameCount;
		static int framePerSecond;
		// static int drawCall;
		static void updateTimeValue();
	};

}