#pragma once
#include <GLFW/glfw3.h>
#include <chrono>
namespace EasyEngine 
{
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
	private:
		void Initialize();
		std::chrono::steady_clock::time_point	__START_TIME;
		std::chrono::steady_clock::time_point	__LAST_TIME;
		std::chrono::steady_clock::time_point	__CURRENT_TIME;
		std::chrono::duration<double>			__ELAPSED;
	};

}