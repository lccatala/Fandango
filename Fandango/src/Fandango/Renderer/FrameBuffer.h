#pragma once

#include "Fandango/Core/BaseDefines.h"

namespace Fandango
{

	enum class FrameBufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		Depth = DEPTH24STENCIL8
	};

	struct FrameBufferTextureSpec
	{
		FrameBufferTextureSpec() = default;
		FrameBufferTextureSpec(FrameBufferTextureFormat format)
			: TextureFormat(format) {}

		FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;
		
		// TODO: filtering/wrap
	};

	struct FrameBufferAttachmentSpec
	{
		FrameBufferAttachmentSpec() = default;
		FrameBufferAttachmentSpec(const std::initializer_list<FrameBufferTextureSpec> attachments)
			: Attachments(attachments) {}

		std::vector<FrameBufferTextureSpec> Attachments;
	};

	// Specification to initialize FrameBuffer
	struct FrameBufferSpec
	{
		uint32_t Width = 0, Height = 0;
		uint32_t Samples = 1;

		FrameBufferAttachmentSpec AttachmentSpec;

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
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual const FrameBufferSpec GetSpec() const = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
	};
}