#pragma once

namespace DZB
{
	class Input
	{

#define MAX_KEYS 512
#define MAX_BUTTONS 8

	private:
		static inline bool s_Keys[MAX_KEYS];
		static inline bool s_Buttons[MAX_BUTTONS];

		static inline bool s_KeyDown[MAX_KEYS];
		static inline bool s_ButtonDown[MAX_BUTTONS];

		static inline bool s_KeyUp[MAX_KEYS];
		static inline bool s_ButtonUp[MAX_BUTTONS];

		static inline glm::vec2 s_MousePos, s_LastMousePos;
		static inline bool s_CursorVisible = false;
	private:
		static void Init()
		{
			for (int i = 0; i < MAX_KEYS; i++)
			{
				s_Keys[i] = false;
			}
			for (int i = 0; i < MAX_BUTTONS; i++)
			{
				s_Buttons[i] = false;
			}
			s_MousePos = glm::vec2();
			s_LastMousePos = glm::vec2();
		}
		static void Update()
		{
			s_LastMousePos = s_MousePos;
		}
	private:
		friend class Screen;
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			s_Keys[key] = action != GLFW_RELEASE;
		}
		static void MouseCallback(GLFWwindow* window, int button, int action, int mods)
		{
			s_Buttons[button] = action != GLFW_RELEASE;
		}
		static void CursorCallback(GLFWwindow* window, double xpos, double ypos)
		{
			s_MousePos.x = (float)xpos;
			s_MousePos.y = (float)ypos;
		}
		static void FocusCallback(GLFWwindow* window, int focused)
		{
			/*
			if (focused)
			{
				Screen::SetCursorMode(GLFW_CURSOR_DISABLED);
			}
			else
			{
				Screen::SetCursorMode(GLFW_CURSOR_NORMAL);
			}
			*/
		}

	public:
		const static bool IsKeyDown(unsigned int keycode)
		{
			if (keycode >= MAX_KEYS)
				return false;
			return s_Keys[keycode];
		}

		const static bool IsButtonDown(unsigned int button)
		{
			if (button >= MAX_BUTTONS)
				return false;
			return s_Buttons[button];
		}

		const static glm::vec2 MousePosition()
		{
			return s_MousePos;
		}

		const static glm::vec2 MouseMovement()
		{
			return s_MousePos - s_LastMousePos;
		}
	};
}