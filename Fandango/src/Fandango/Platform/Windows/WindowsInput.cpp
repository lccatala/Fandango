#include "fndgpch.h"

#include "WindowsInput.h"
#include "Fandango/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Fandango
{
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);

		return (state == GLFW_PRESS || state == GLFW_REPEAT);
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		return { (float)x, (float)y };
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto[xpos, ypos] = GetMousePositionImpl();
		return (float)xpos;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [xpos, ypos] = GetMousePositionImpl();
		return (float)ypos;
	}
}