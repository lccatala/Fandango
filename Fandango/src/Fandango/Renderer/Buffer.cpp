#include "fndgpch.h"

#include "Buffer.h"
#include "Renderer.h"

#include "Fandango/Platform/OpenGL/OpenGLBuffer.h"

namespace Fandango
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		// Decide which class to instantiate based on the API we're using
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			FNDG_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(size);
		}

		FNDG_ENGINE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		// Decide which class to instantiate based on the API we're using
		switch (Renderer::GetAPI()) 
		{
		case RendererAPI::API::None:
			FNDG_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		FNDG_ENGINE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		// Decide which class to instantiate based on the API we're using
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			FNDG_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLIndexBuffer>(indices, size);
		}

		FNDG_ENGINE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	} 
}