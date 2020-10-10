#include <Fandango.h>
#include <Fandango/Core/EntryPoint.h>

#include "Fandango/Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

#include "EditorLayer.h"

namespace Fandango
{
	class Editor : public Application
	{
	public:
		Editor()
			: Application("Fandango Editor")
		{
			PushLayer(new EditorLayer());
		}

		~Editor()
		{

		}
	};


	Application* CreateApplication()
	{
		return new Editor();
	}
}

