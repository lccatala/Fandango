#pragma once

#include "Fandango/Renderer/FrameBuffer.h"

namespace Fandango
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpec& spec);
		virtual ~OpenGLFrameBuffer();

		void Bind() override;
		void Unbind() override;

		const FrameBufferSpec GetSpec() const override { return m_Spec; };
		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; };

	private:
		void Recreate();


		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		FrameBufferSpec m_Spec;
	};
}