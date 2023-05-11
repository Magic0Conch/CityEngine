#pragma once
#include <GLFW/glfw3.h>
#include <chrono>
namespace EasyEngine::Tool
{
	class Clock
	{
	public:
        Clock() = default;

        void update();
        float getFPS() const;
        float getDeltaTime() const;
        float getTimeScale() const;
        float getTimeScaleUnscaled() const;
        float getTimeSinceStart() const;
        void setTimeScale(float p_timeScale);

	private:
		void initialize();
		std::chrono::steady_clock::time_point __startTime;
		std::chrono::steady_clock::time_point __lastTime;
		std::chrono::steady_clock::time_point __currentTime;
		std::chrono::duration<double> __elapsed;

        bool m_initialized = false;
		float m_timeScale = 1.0f;
		float m_deltaTime = 0.0f;
		float m_timeSinceStart = 0.0f;
	};

}