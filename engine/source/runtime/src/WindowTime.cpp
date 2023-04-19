#include "../include/WindowTime.h"
namespace EasyEngine {
	double WindowTime::currentTimeValue;
	double WindowTime::lastTimeValue;
	double WindowTime::deltaTimeValue;
	int WindowTime::frameCount;
	int WindowTime::framePerSecond;
	// int WindowTime::drawCall;

	//call per frame
	void WindowTime::updateTimeValue() {
		lastTimeValue = currentTimeValue;
		currentTimeValue = glfwGetTime();
		deltaTimeValue = currentTimeValue - lastTimeValue;
		++frameCount;
		framePerSecond = 1.0 / deltaTimeValue;
		// drawCall = 0;
	}
}