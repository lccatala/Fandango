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

		glm::vec2 m_ViewportSize = {0.0f, 0.0f};
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		OrthographicCameraController m_CameraController;

		Ref<FrameBuffer> m_FrameBuffer;
		Ref<Scene> m_ActiveScene;
		Entity m_CameraEntity;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;


		// Temporary
		bool m_PrimaryCamera = true;
		glm::vec4 m_SquareColor = { 0.2, 0.3f, 0.8f, 1.0f };
		Entity m_SquareEntity, m_RedSquareEntity;
	};
}