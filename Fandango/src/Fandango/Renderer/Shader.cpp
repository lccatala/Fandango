#include "fndgpch.h"
#include "Shader.h"
#include "Renderer.h"

#include "Fandango/Platform/OpenGL/OpenGLShader.h"

namespace Fandango
{
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			FNDG_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLShader(vertexSrc, fragmentSrc);
		}
	}
}