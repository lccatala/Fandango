#pragma once

#include "Fandango/Core/BaseDefines.h"

namespace Fandango
{

	// Specification to initialize FrameBuffer
	struct FrameBufferSpec
	{
		uint32_t Width = 0, Height = 0;
		uint32_t Samples = 1;

		// Is supposed to be rendering to Vulkan SwapChain?
		// Are we rendering directly to the screen or not?
		bool SwapChainTarget = false;

		// TODO: FrameBufferFormat
	};

	class FrameBuffer
	{
	public:
		static Ref<FrameBuffer> Create(const FrameBufferSpec& spec);
		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual const FrameBufferSpec GetSpec() const = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;
	};
}