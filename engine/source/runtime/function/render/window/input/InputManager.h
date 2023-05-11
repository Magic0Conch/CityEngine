#pragma once

#include "runtime/function/render/window/Window.h"
#include <tool/event/Event.h>
#include "InputType.h"
namespace EasyEngine::Input {
	class InputManager
	{
	public:
		InputManager(Window& p_window);
		~InputManager();

		KeyState getKeyState(Key p_key) const;
		MouseButtonState getMouseButtonState(MouseButton p_button) const;

		bool isKeyPressed(Key p_key) const;

		bool isKeyReleased(Key p_key) const;

		bool isMouseButtonPressed(MouseButton p_button) const;

		bool isMouseButtonReleased(MouseButton p_button) const;

		std::pair<double, double> getMousePosition() const;

		void clearEvents();

	private:
		void onKeyPressed(int p_key);
		void onKeyReleased(int p_key);
		void onMouseButtonPressed(int p_button);
		void onMouseButtonReleased(int p_button);

	private:
		Window& m_window;

		Tool::ListenerID m_keyPressedListener;
		Tool::ListenerID m_keyReleasedListener;
		Tool::ListenerID m_mouseButtonPressedListener;
		Tool::ListenerID m_mouseButtonReleasedListener;

		std::unordered_map<Key, KeyState> m_keyEvents;
		std::unordered_map<MouseButton, MouseButtonState> m_mouseButtonEvents;
	};
}