#pragma once

#include "Fandango/Renderer/FrameBuffer.h"

namespace Fandango
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpec& spec);
		virtual ~OpenGLFrameBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		const FrameBufferSpec GetSpec() const override { return m_Spec; };
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override 
		{ 
			FNDG_ENGINE_ASSERT(index < m_ColorAttachments.size());
			return m_ColorAttachments[index]; 
		};

	private:
		void Recreate();


		uint32_t m_RendererID = 0;
		FrameBufferSpec m_Spec;
		std::vector<FrameBufferTextureSpec> m_ColorAttachmentSpecs;
		FrameBufferTextureSpec m_DepthAttachmentSpec = FrameBufferTextureFormat::None;
		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}