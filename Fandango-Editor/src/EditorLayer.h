#pragma once

#include <Fandango.h>
#include "Panels/SceneHierarchyPanel.h"

namespace Fandango
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(TimeStep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event & e) override;
	private:
		void SetupDockspace();
		void DrawGuizmos();
		bool OnKeyPressed(KeyPressedEvent& e);
		void CreateNewScene();
		void OpenScene();
		void SaveSceneAs();

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		int m_GizmoType = -1;

		glm::vec2 m_ViewportSize = {0.0f, 0.0f};

		OrthographicCameraController m_CameraController;

		Ref<FrameBuffer> m_FrameBuffer;
		Ref<Scene> m_ActiveScene;
		Entity m_CameraEntity;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}