#include "fndgpch.h"
#include "RenderCommand.h"

#include "Fandango/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Fandango
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}