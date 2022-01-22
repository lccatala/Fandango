#include "EditorLayer.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Fandango/Math/Math.h>
#include <Fandango/Scene/SceneSerializer.h>
#include "Fandango/Utils/PlatformUtils.h"

#include "ImGuizmo.h"

namespace Fandango
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f)
	{}

	void EditorLayer::OnAttach()
	{
		FNDG_PROFILE_FUNCTION();

		FrameBufferSpec fbSpec;
		fbSpec.AttachmentSpec = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();
		m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

#if 0 
		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate() override
			{
				
			}

			void OnDestroy() override
			{

			}

			void OnUpdate(TimeStep ts) override
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;

				if (Input::IsKeyPressed(FNDG_KEY_A))
					translation.x -= speed * ts;
				if (Input::IsKeyPressed(FNDG_KEY_D))
					translation.x += speed * ts;
				if (Input::IsKeyPressed(FNDG_KEY_W))
					translation.y += speed * ts;
				if (Input::IsKeyPressed(FNDG_KEY_S))
					translation.y -= speed * ts;
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
		FNDG_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(TimeStep ts)
	{
		FNDG_PROFILE_FUNCTION();

		if (FrameBufferSpec spec = m_FrameBuffer->GetSpec();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.Resize(m_ViewportSize.x, m_ViewportSize.y);

			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		if (m_ViewportFocused)
		{
			m_CameraController.OnUpdate(ts);
			m_EditorCamera.OnUpdate(ts);
		}

		Renderer2D::ResetStats();

		m_FrameBuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		// Clear Entity ID Attachment to -1
		m_FrameBuffer->ClearAttachmentInt(1, -1);

		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

		auto [mouseXDouble, mouseYDouble] = ImGui::GetMousePos();

		// Top-left corner is (0,0)
		mouseXDouble -= m_ViewportBounds[0].x;
		mouseYDouble -= m_ViewportBounds[0].y;

		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		mouseYDouble = viewportSize.y - mouseYDouble; // Flip Y coordinate so bottom-left is (0,0)

		int mouseX = (int)mouseXDouble;
		int mouseY = (int)mouseYDouble;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
			FNDG_ENGINE_TRACE("Pixel data: {0}", pixelData);
		}

		m_FrameBuffer->Unbind();
	}

	void EditorLayer::CreateNewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Fandango Scene (*.fndg)\0*.fndg\0");
		if (!filepath.empty())
		{
			CreateNewScene();
			SceneSerializer serializer(m_ActiveScene);
			serializer.DeSerializeText(filepath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Fandango Scene (*.fndg)\0*.fndg\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.SerializeText(filepath);
		}
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
		ImGuiStyle& style = ImGui::GetStyle();
		float minWindowSize = style.WindowMinSize.x;
		style.WindowMinSize.x = 350.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}

		style.WindowMinSize.x = minWindowSize;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					CreateNewScene();
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}

				if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}

				if (ImGui::MenuItem("Quit", NULL, false))
				{
					showDockspace = false;
					Application::Get().Close();
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

	void EditorLayer::DrawGizmos()
	{
		// Get currently selected entity
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (!selectedEntity || m_GizmoType == -1)
			return;

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		float windowX = ImGui::GetWindowPos().x;
		float windowY = ImGui::GetWindowPos().y;
		float windowWidth = ImGui::GetWindowWidth();
		float windowHeight = ImGui::GetWindowHeight();
		ImGuizmo::SetRect(windowX, windowY, windowWidth, windowHeight);

		// Get entity transform
		auto& entityTransformComponent = selectedEntity.GetComponent<TransformComponent>();
		glm::mat4 entityTransform = entityTransformComponent.GetTransform();

		// Snapping to 0.5m for translation/scale and 45 degrees for rotation
		// Note: snapping is relative to current position, not to a global grid
		bool snap = Input::IsKeyPressed(FNDG_KEY_LEFT_CONTROL);
		float snapValue = m_GizmoType == ImGuizmo::OPERATION::ROTATE ? 45.0f : 0.5f;
		float snapValues[3] = { snapValue , snapValue , snapValue }; // Same snap values for every axis

		ImGuizmo::Manipulate(
			glm::value_ptr(m_EditorCamera.GetViewMatrix()), 
			glm::value_ptr(m_EditorCamera.GetProjection()), 
			(ImGuizmo::OPERATION)m_GizmoType,
			ImGuizmo::LOCAL, 
			glm::value_ptr(entityTransform),
			nullptr,
			snap ? snapValues : nullptr);

		if (!ImGuizmo::IsUsing())
			return;

		glm::vec3 translation, rotation, scale;
		Math::DecomposeTransform(entityTransform, translation, rotation, scale);

		glm::vec3 deltaRotation = rotation - entityTransformComponent.Rotation;
		entityTransformComponent.Translation = translation;
		entityTransformComponent.Rotation += deltaRotation;
		entityTransformComponent.Scale = scale;
	}

	void EditorLayer::OnImGuiRender()
	{
		FNDG_PROFILE_FUNCTION();

		SetupDockspace();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});

		m_SceneHierarchyPanel.OnImGuiRender();

		ImGui::Begin("Viewport");
		auto viewportOffset = ImGui::GetCursorPos(); // Includes tab bar

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 currentViewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { currentViewportPanelSize.x, currentViewportPanelSize.y };

		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		auto windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		m_ViewportBounds[0] = { minBound.x, minBound.y };
		m_ViewportBounds[1] = { maxBound.x, maxBound.y };


		DrawGizmos();

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(FNDG_BIND_EVENT_FUNCTION(EditorLayer::OnKeyPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Keyboard shortcuts
		if (e.GetRepeatCount() > 0)
			return false;

		bool controlPressed = Input::IsKeyPressed(FNDG_KEY_LEFT_CONTROL) || Input::IsKeyPressed(FNDG_KEY_RIGHT_CONTROL);
		bool shiftPressed = Input::IsKeyPressed(FNDG_KEY_LEFT_SHIFT) || Input::IsKeyPressed(FNDG_KEY_RIGHT_SHIFT);

		switch (e.GetKeyCode())
		{
		case FNDG_KEY_N:
			if (controlPressed)
				CreateNewScene();
			break;

		case FNDG_KEY_O:
			if (controlPressed)
				OpenScene();
			break;

		case FNDG_KEY_S:
			if (controlPressed && shiftPressed)
				SaveSceneAs();
			break;

		// Guizmo shortcuts
		case FNDG_KEY_Q:
			m_GizmoType = -1;
			break;

		case FNDG_KEY_W:
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;

		case FNDG_KEY_E:
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;

		case FNDG_KEY_R:
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		}
		
	}
}

