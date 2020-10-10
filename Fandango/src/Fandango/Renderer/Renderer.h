#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Fandango
{
	class Renderer
	{
	public:
		static void Init();

		static void BeginScene(OrthographicCamera &camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader> &shader, const glm::mat4 &transform = glm::mat4(1.0f));

		static void OnWindowResize(uint32_t width, uint32_t height);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}