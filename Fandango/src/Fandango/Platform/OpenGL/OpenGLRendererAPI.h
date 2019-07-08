#pragma once
#include "fndgpch.h"
#include "Fandango/Renderer/RendererAPI.h"

namespace Fandango
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}