#include "Sandbox2D.h"
#include <Fandango/Core/EntryPoint.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Fandango/Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{}

void Sandbox2D::OnAttach()
{
	m_SquareVA = Fandango::VertexArray::Create();

	float squareVertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	std::shared_ptr<Fandango::VertexBuffer> squareVB;
	squareVB.reset(Fandango::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{Fandango::ShaderDataType::Float3, "a_Position"}
	});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	std::shared_ptr<Fandango::IndexBuffer> squareIB;
	squareIB.reset(Fandango::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_FlatColorShader = Fandango::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{}

void Sandbox2D::OnUpdate(Fandango::TimeStep ts)
{
	m_CameraController.OnUpdate(ts);

	Fandango::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Fandango::RenderCommand::Clear();

	Fandango::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Fandango::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Fandango::OpenGLShader>(m_FlatColorShader)->UploadUniform("u_Color", m_SquareColor);

	Fandango::Renderer::Submit(m_SquareVA, m_FlatColorShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Fandango::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Fandango::Event& e)
{
	m_CameraController.OnEvent(e);
}