#include "fndgpch.h"
#include "Renderer.h"
#include "Shader.h"

namespace Fandango
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera &camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	
	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const glm::mat4& transform)
	{
		shader->Bind();
		shader->UploadUniform("u_ViewProjectionMatrix", m_SceneData->ViewProjectionMatrix);
		shader->UploadUniform("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}