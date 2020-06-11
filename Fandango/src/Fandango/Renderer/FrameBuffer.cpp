#include <fndgpch.h>

#include "FrameBuffer.h"
#include "Fandango/Renderer/Renderer.h"
#include "Fandango/Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Fandango
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpec& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			FNDG_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFrameBuffer>(spec);
		}
	}
}