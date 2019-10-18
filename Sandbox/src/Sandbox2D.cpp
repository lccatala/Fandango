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
	
}

void Sandbox2D::OnDetach()
{}

void Sandbox2D::OnUpdate(Fandango::TimeStep ts)
{
	m_CameraController.OnUpdate(ts);

	Fandango::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Fandango::RenderCommand::Clear();

	Fandango::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Fandango::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0, 1.0f }, { 0.8f, 0.2f, 0.2f, 0.1f });

	Fandango::Renderer::EndScene();
	//std::dynamic_pointer_cast<Fandango::OpenGLShader>(m_FlatColorShader)->Bind();
	//std::dynamic_pointer_cast<Fandango::OpenGLShader>(m_FlatColorShader)->UploadUniform("u_Color", m_SquareColor);
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