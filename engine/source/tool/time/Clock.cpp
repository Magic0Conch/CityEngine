#include "Clock.h"
#include <chrono>

using namespace EasyEngine::Tool;
void Clock::update(){
    __lastTime = __currentTime;
    __currentTime = std::chrono::steady_clock::now();
    __elapsed = __currentTime - __lastTime;
    if(m_initialized){
        m_deltaTime = __elapsed.count();
        m_timeSinceStart += m_deltaTime * m_timeScale;
    }
    else{
        initialize();
    }
}   

float Clock::getFPS() const{
    return 1.0/m_deltaTime;
}

float Clock::getDeltaTime() const{
    return m_deltaTime*m_timeScale;
}
float Clock::getTimeScaleUnscaled() const{
    return m_deltaTime;
}

float Clock::getTimeScale() const{
    return m_timeScale;
}

float Clock::getTimeSinceStart() const{
    return m_timeSinceStart;
}

void Clock::setTimeScale(float p_timeScale){
    m_timeScale = p_timeScale;
}

void Clock::initialize(){
    m_deltaTime = 0.0f;
    __startTime = std::chrono::steady_clock::now();
    __currentTime = __startTime;
    __lastTime = __startTime;
    m_initialized = true;
}