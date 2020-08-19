#include "fndgpch.h"

#include "Fandango/Core/Input.h"
#include "Fandango/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Fandango
{
	bool Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);

		return (state == GLFW_PRESS || state == GLFW_REPEAT);
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		return { (float)x, (float)y };
	}

	float Input::GetMouseX()
	{
		auto[xpos, ypos] = GetMousePosition();
		return (float)xpos;
	}

	float Input::GetMouseY()
	{
		auto [xpos, ypos] = GetMousePosition();
		return (float)ypos;
	}
}