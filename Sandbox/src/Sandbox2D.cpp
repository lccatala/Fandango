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
	FNDG_PROFILE_FUNCTION();

	m_Texture = Fandango::Texture2D::Create("assets/textures/checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	FNDG_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Fandango::TimeStep ts)
{
	FNDG_PROFILE_FUNCTION();
	
	m_CameraController.OnUpdate(ts);

	Fandango::Renderer2D::ResetStats();
	
	Fandango::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Fandango::RenderCommand::Clear();

	Fandango::Renderer2D::BeginScene(m_CameraController.GetCamera());

	//Fandango::Renderer2D::DrawQuad({ 0.5f, 0.5f }, 0.0f, { 0.5f, 0.75f }, m_SquareColor);
	//Fandango::Renderer2D::DrawQuad({ 1.5f, 0.5f }, 0.0f, { 0.5f, 0.75f }, {1.0f, 0.0f, 0.0f, 1.0f});
	Fandango::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, 0.0f, 10.0f, { 11.0f, 11.0f }, m_Texture);


	for (float y = -5.0f; y < 5.0f; y += 0.5f)
	{
		for (float x = -5.0f; x < 5.0f; x += 0.5f)
		{
			glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f };
			Fandango::Renderer2D::DrawQuad({ x, y }, 0.0f, { 0.45f, 0.45f }, color);
		}
	}
	Fandango::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	FNDG_PROFILE_FUNCTION();

	auto stats = Fandango::Renderer2D::GetStats();

	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor));
	ImGui::Text("Renderer2D Stats");
	ImGui::Text("Draw calls: %d", stats.DrawCalls);
	ImGui::Text("Quad count: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::End();
}

void Sandbox2D::OnEvent(Fandango::Event& e)
{
	m_CameraController.OnEvent(e);
}