#include "InputManager.h"
#include "GLFW/glfw3.h"

namespace EasyEngine::Input {

	InputManager::InputManager(Window& p_window):m_window(p_window){
		m_keyPressedListener = m_window.keyPressedEvent.addListener(std::bind(&InputManager::onKeyPressed, this, std::placeholders::_1));
		m_keyReleasedListener = m_window.keyReleasedEvent.addListener(std::bind(&InputManager::onKeyReleased, this, std::placeholders::_1));
		m_mouseButtonPressedListener = m_window.mouseButtonPressedEvent.addListener(std::bind(&InputManager::onMouseButtonPressed, this, std::placeholders::_1));
		m_mouseButtonReleasedListener = m_window.mouseButtonReleasedEvent.addListener(std::bind(&InputManager::onMouseButtonReleased, this, std::placeholders::_1));

	}
	InputManager::~InputManager(){
		m_window.keyPressedEvent.removeListener(m_keyPressedListener);
		m_window.keyReleasedEvent.removeListener(m_keyReleasedListener);
		m_window.mouseButtonPressedEvent.removeListener(m_mouseButtonPressedListener);
		m_window.mouseButtonReleasedEvent.removeListener(m_mouseButtonReleasedListener);
	}

	KeyState InputManager::getKeyState(Key p_key) const{
		switch (glfwGetKey(m_window.getGlfwWindow(), static_cast<int>(p_key))) {
			case GLFW_PRESS:
				return KeyState::KEY_DOWN;
			case GLFW_RELEASE:
				return KeyState::KEY_UP;
		}
		return KeyState::KEY_UP;
	}

	MouseButtonState InputManager::getMouseButtonState(MouseButton p_button) const{
		switch (glfwGetMouseButton(m_window.getGlfwWindow(), static_cast<int>(p_button))) {
			case GLFW_PRESS:
				return MouseButtonState::MOUSE_DOWN;
			case GLFW_RELEASE:
				return MouseButtonState::MOUSE_UP;
		}
		return MouseButtonState::MOUSE_UP;
	}

	bool InputManager::isKeyPressed(Key p_key) const{
		return m_keyEvents.find(p_key)!=m_keyEvents.end() && m_keyEvents.at(p_key) == KeyState::KEY_DOWN;
	}

	bool InputManager::isKeyReleased(Key p_key) const{
		return m_keyEvents.find(p_key)!=m_keyEvents.end() && m_keyEvents.at(p_key) == KeyState::KEY_UP;
	}

	bool InputManager::isMouseButtonPressed(MouseButton p_button) const{
		return m_mouseButtonEvents.find(p_button)!=m_mouseButtonEvents.end() && m_mouseButtonEvents.at(p_button) == MouseButtonState::MOUSE_DOWN;
	}

	bool InputManager::isMouseButtonReleased(MouseButton p_button) const{
		return m_mouseButtonEvents.find(p_button)!=m_mouseButtonEvents.end() && m_mouseButtonEvents.at(p_button) == MouseButtonState::MOUSE_UP;
	}

	std::pair<double, double> InputManager::getMousePosition() const{
		std::pair<double, double> mousePosition;
		glfwGetCursorPos(m_window.getGlfwWindow(), &mousePosition.first, &mousePosition.second);
		return mousePosition;
	}

	void InputManager::clearEvents(){
		m_keyEvents.clear();
		m_mouseButtonEvents.clear();
	}

	void InputManager::onKeyPressed(int p_key){
		m_keyEvents[static_cast<Key>(p_key)] = KeyState::KEY_DOWN;
	}
	void InputManager::onKeyReleased(int p_key){
		m_keyEvents[static_cast<Key>(p_key)] = KeyState::KEY_UP;
	}
	void InputManager::onMouseButtonPressed(int p_button){
		m_mouseButtonEvents[static_cast<MouseButton>(p_button)] = MouseButtonState::MOUSE_DOWN;
	}
	void InputManager::onMouseButtonReleased(int p_button){
		m_mouseButtonEvents[static_cast<MouseButton>(p_button)] = MouseButtonState::MOUSE_UP;		
	}
}

