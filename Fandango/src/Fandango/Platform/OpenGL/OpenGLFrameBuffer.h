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
		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; };

	private:
		void Recreate();


		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
		FrameBufferSpec m_Spec;
	};
}