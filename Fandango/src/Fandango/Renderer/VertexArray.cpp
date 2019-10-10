#include "fndgpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Fandango/Platform/OpenGL/OpenGLVertexArray.h"


namespace Fandango
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			FNDG_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		}

		FNDG_ENGINE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}
}