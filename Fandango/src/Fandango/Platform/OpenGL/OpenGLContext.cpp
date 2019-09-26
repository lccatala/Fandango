#include "fndgpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Fandango
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		FNDG_ENGINE_ASSERT(windowHandle, "windowHandle is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		FNDG_ENGINE_ASSERT(status, "Failed to initialized Glad");

		FNDG_ENGINE_INFO("OpenGL Vendor:   {0}", glGetString(GL_VENDOR));
		FNDG_ENGINE_INFO("OpenGL Renderer: {0}", glGetString(GL_RENDERER));

		int majorVersion, minorVersion;
		glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
		glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

		FNDG_ENGINE_ASSERT(majorVersion >= 4 && minorVersion >= 5, "Fandango requires at least OpenGL version 4.5");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}