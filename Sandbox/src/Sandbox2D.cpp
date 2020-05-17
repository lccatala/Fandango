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

	m_SpriteSheet = Fandango::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");
	m_BarrelSubTexture = Fandango::SubTexture2D::CreateFromCoords(m_SpriteSheet, {8, 2}, {128, 128});
	m_StairsSubTexture = Fandango::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 6 }, { 128, 128 });
	m_TreeSubTexture = Fandango::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 128, 128 }, { 1, 2 });

	m_ParticleSystem = Fandango::ParticleSystem();

	m_ParticleProps.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_ParticleProps.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_ParticleProps.SizeBegin = 0.5f;
	m_ParticleProps.SizeVariation = 0.3f;
	m_ParticleProps.SizeEnd = 0.0f;
	m_ParticleProps.LifeTime = 1.0f;
	m_ParticleProps.Velocity = { 0.0f, 0.0f };
	m_ParticleProps.VelocityVariation = { 3.0f, 1.0f };
	m_ParticleProps.Position = { 0.0f, 0.0f };
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
	Fandango::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, 0.0f, 1.0f, { 1.0f, 1.0f }, m_StairsSubTexture);
	Fandango::Renderer2D::DrawQuad({ 1.0f, 0.0f, -0.1f }, 0.0f, 1.0f, { 1.0f, 1.0f }, m_BarrelSubTexture);
	Fandango::Renderer2D::DrawQuad({ 2.0f, 0.0f, -0.1f }, 0.0f, 1.0f, { 1.0f, 2.0f }, m_TreeSubTexture);
	Fandango::Renderer2D::EndScene();

#ifdef ENABLE_PARTICLES
	if (Fandango::Input::IsMouseButtonPressed(FNDG_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Fandango::Input::GetMousePosition();
		auto width = Fandango::Application::Get().GetWindow().GetWidth();
		auto height = Fandango::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetCameraBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_ParticleProps.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 10; i++)
			m_ParticleSystem.Emit(m_ParticleProps);
	}
	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());
#endif
}

void Sandbox2D::OnImGuiRender()
{
	FNDG_PROFILE_FUNCTION();

	//auto stats = Fandango::Renderer2D::GetStats();

	ImGui::Begin("Settings");
	
	ImGui::ColorEdit4("Birth color", glm::value_ptr(m_ParticleProps.ColorBegin));
	ImGui::ColorEdit4("Death color", glm::value_ptr(m_ParticleProps.ColorEnd));
	ImGui::DragFloat("Lifetime", &m_ParticleProps.LifeTime, 0.1f, 0.0f, 1000.0f);
	ImGui::End();
}

void Sandbox2D::OnEvent(Fandango::Event& e)
{
	m_CameraController.OnEvent(e);
}