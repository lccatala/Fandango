#include "fndgpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Fandango/Platform/OpenGL/OpenGLVertexArray.h"


namespace Fandango
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			FNDG_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLVertexArray();
		}

		FNDG_ENGINE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}
}