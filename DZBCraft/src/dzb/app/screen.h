#pragma once

#include <GLFW/glfw3.h>
#include <GLEW/glew.h>

#include <iostream>
#include <vector>

#include "input.h"

namespace DZB
{
	class ScreenResizeListener
	{
		
	private:
		friend class Screen;
		virtual void ResizeCallback(GLFWwindow* window, int width, int height) = 0;
	};

	class Screen
	{
		
	private:
		int m_Width, m_Height;
		const char* m_Title;
		GLFWwindow* m_Window;
		std::vector<ScreenResizeListener*> m_ResizeListeners;
	private:
		Screen(GLFWwindow* window, const char* title, int width, int height) :
			m_Window(window),
			m_Title(title),
			m_Width(width),
			m_Height(height)
		{ }

		~Screen()
		{
			std::cout << "Terminating Screen" << std::endl;
			glfwTerminate();
		}

	private:
		static inline Screen* instance;
		static void ResizeCallback(GLFWwindow* window, int width, int height)
		{
			instance->m_Width = width;
			instance->m_Height = height;
			glViewport(0, 0, width, height);
			for (auto& i : instance->m_ResizeListeners)
				i->ResizeCallback(window, width, height);
		}
	public:
		static void Create(const char* title, int width, int height)
		{
			if (instance != nullptr)
			{
				delete instance;
			}

			if (!glfwInit())
			{
				std::cout << "Failed to initialize GLFW" << std::endl;
				glfwTerminate();
				return;
			}

			GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
			if (!window) {
				glfwTerminate();
				std::cout << "Failed to create GLFW Window" << std::endl;
				glfwTerminate();
				return;
			}
			glfwMakeContextCurrent(window);

			glfwSwapInterval(0);

			if (glewInit() != GLEW_OK) {
				std::cout << "Failed to initialize GLEW" << std::endl;
				return;
			}

			std::cout << "OpenGL Version " << glGetString(GL_VERSION) << std::endl;

			std::cout << "Graphics Hardware " << glGetString(GL_VENDOR) << " " << glGetString(GL_RENDERER) << std::endl;


			instance = new Screen(window, title, width, height);

			Input::Init();
			glfwSetWindowSizeCallback(window, ResizeCallback);
			glfwSetKeyCallback(window, Input::KeyCallback);
			glfwSetMouseButtonCallback(window, Input::MouseCallback);
			glfwSetCursorPosCallback(window, Input::CursorCallback);
			glfwSetWindowFocusCallback(window, Input::FocusCallback);
			glfwSetWindowUserPointer(window, instance);
		}

		static const void AddListener(ScreenResizeListener* listener)
		{
			instance->m_ResizeListeners.push_back(listener);
		}

		static const inline char* GetTitle() { return instance == nullptr ? "" : instance->m_Title; }
		static const inline int GetWidth() { return instance == nullptr ? 0 : instance->m_Width; }
		static const inline int GetHeight() { return instance == nullptr ? 0 : instance->m_Height; }
		static const bool Active() { return !glfwWindowShouldClose(instance->m_Window); }
		static const void Update()
		{
			Input::Update();
			GLenum error = glGetError();
			if (error != GL_NO_ERROR)
				std::cout << "OpenGL Error: " << error << std::endl;

			glfwSwapBuffers(instance->m_Window);
			glfwPollEvents();
		}

		static const void Clear()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		static const int GetCursorMode()
		{
			return glfwGetInputMode(instance->m_Window, GLFW_CURSOR);
		}

		static const void SetCursorMode(int mode)
		{
			glfwSetInputMode(instance->m_Window, GLFW_CURSOR, mode);
		}
	};
}