#include "fndgpch.h"
#include "WindowsWindow.h"

#include "Fandango/Events/ApplicationEvent.h"
#include "Fandango/Events/KeyEvent.h"
#include "Fandango/Events/MouseEvent.h"

#include "Fandango/Platform/OpenGL/OpenGLContext.h"

namespace Fandango
{
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		FNDG_ENGINE_ERROR("GLFW error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProperties& properties)
	{
	#ifdef FNDG_PLATFORM_WINDOWS
		return new WindowsWindow(properties);
	#else
		FNDG_ENGINE_ASSERT(false, "Unknown platform");
		return nullptr;
	#endif
	}

	WindowsWindow::WindowsWindow(const WindowProperties& properties)
	{
		Init(properties);
	}

	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}

	void WindowsWindow::Init(const WindowProperties& properties)
	{
		FNDG_PROFILE_FUNCTION();
		m_Data.Title = properties.m_Title;
		m_Data.Width = properties.m_Width;
		m_Data.Height = properties.m_Height;

		if (s_GLFWWindowCount == 0)
		{
			int success = glfwInit();
			
			FNDG_ENGINE_ASSERT(success, "Could not initialize GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		m_Window = glfwCreateWindow((int)properties.m_Width, (int)properties.m_Height, m_Data.Title.c_str(), nullptr, nullptr);
		s_GLFWWindowCount++;

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// GLFW event callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					break;
				}
				case GLFW_REPEAT:
				{
					// GLFW doesn't provide a repeat count, although we might abstract that from the Win32 API
					KeyPressedEvent event(key, 1);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseWheelScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		FNDG_PROFILE_FUNCTION();
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.Vsync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.Vsync;
	}

	void WindowsWindow::ShutDown()
	{
		FNDG_PROFILE_FUNCTION();
		glfwDestroyWindow(m_Window);

		if (--s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		FNDG_PROFILE_FUNCTION();
		glfwPollEvents();
		m_Context->SwapBuffers();
	}
}
