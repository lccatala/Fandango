#include "EditorLayer.h"
#include <Fandango/Core/EntryPoint.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Fandango/Platform/OpenGL/OpenGLShader.h"

namespace Fandango
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f)
	{}

	void EditorLayer::OnAttach()
	{
		FNDG_PROFILE_FUNCTION();

		m_CheckerboardTexture = Fandango::Texture2D::Create("assets/textures/Checkerboard.png");
		m_SpriteSheet = Fandango::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");
		Fandango::FrameBufferSpec fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = Fandango::FrameBuffer::Create(fbSpec);
	}

	void EditorLayer::OnDetach()
	{
		FNDG_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Fandango::TimeStep ts)
	{
		FNDG_PROFILE_FUNCTION();

		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		Fandango::Renderer2D::ResetStats();

		m_FrameBuffer->Bind();

		Fandango::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Fandango::RenderCommand::Clear();

		Fandango::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
			}
		}
		Fandango::Renderer2D::DrawQuadTexture({ 0.0f, 0.0f, 0.0f }, 0.0f, 1.0f, { 0.45f, 0.45f }, m_SpriteSheet);
		Fandango::Renderer2D::EndScene();

		m_FrameBuffer->Unbind();

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

	void EditorLayer::SetupDockspace()
	{
		FNDG_PROFILE_FUNCTION();

		static bool showDockspace = true;
		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		bool opt_fullscreen = opt_fullscreen_persistant;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &showDockspace, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Quit", NULL, false))
				{
					showDockspace = false;
					Fandango::Application::Get().Close();
				}
				ImGui::EndMenu();
			}
			/*
			if (ImGui::BeginMenu("Docking"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Flag: NoSplit", "", (opt_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 opt_flags ^= ImGuiDockNodeFlags_NoSplit;
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (opt_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  opt_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
				if (ImGui::MenuItem("Flag: NoResize", "", (opt_flags & ImGuiDockNodeFlags_NoResize) != 0))                opt_flags ^= ImGuiDockNodeFlags_NoResize;
				if (ImGui::MenuItem("Flag: PassthruDockspace", "", (opt_flags & ImGuiDockNodeFlags_PassthruDockspace) != 0))       opt_flags ^= ImGuiDockNodeFlags_PassthruDockspace;
				ImGui::Separator();
				if (ImGui::MenuItem("Close DockSpace", NULL, false))
					showDockspace = false;
				ImGui::EndMenu();
			}
			*/
			ImGui::EndMenuBar();
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		FNDG_PROFILE_FUNCTION();

		SetupDockspace();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});

		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 currentViewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *(glm::vec2*) & currentViewportPanelSize)
		{
			m_ViewportSize = { currentViewportPanelSize.x, currentViewportPanelSize.y };
			m_FrameBuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
			m_CameraController.Resize(m_ViewportSize.x, m_ViewportSize.y);
		}
		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Fandango::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}

