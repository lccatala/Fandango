#pragma once

#include <Fandango.h>

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

		OrthographicCameraController m_CameraController;
		ParticleSystem m_ParticleSystem;
		ParticleProps m_ParticleProps;

		// Temporary
		Ref<Fandango::VertexArray> m_SquareVA;
		Ref<Fandango::Shader> m_FlatColorShader;

		Ref<Fandango::Texture2D> m_SpriteSheet, m_CheckerboardTexture;
		Ref<Fandango::SubTexture2D> m_BarrelSubTexture, m_StairsSubTexture, m_TreeSubTexture;
		Ref<Fandango::FrameBuffer> m_FrameBuffer;

		glm::vec4 m_SquareColor = { 0.2, 0.3f, 0.8f, 1.0f };
	};
}