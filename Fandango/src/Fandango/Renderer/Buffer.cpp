#include "fndgpch.h"

#include "Buffer.h"
#include "Renderer.h"

#include "Fandango/Platform/OpenGL/OpenGLBuffer.h"

namespace Fandango
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		// Decide which class to instantiate based on the API we're using
		switch (Renderer::GetAPI()) 
		{
		case RendererAPI::API::None:
				FNDG_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return new OpenGLVertexBuffer(vertices, size);
		}

		FNDG_ENGINE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		// Decide which class to instantiate based on the API we're using
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			FNDG_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLIndexBuffer(indices, size);
		}

		FNDG_ENGINE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	} 
}