#pragma once

#include <Fandango.h>

class Sandbox2D : public Fandango::Layer
{
public:
	Sandbox2D();
	~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Fandango::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Fandango::Event& e) override;
private:
	Fandango::OrthographicCameraController m_CameraController;
	Fandango::ParticleSystem m_ParticleSystem;
	Fandango::ParticleProps m_ParticleProps;

	// Temporary
	Fandango::Ref<Fandango::VertexArray> m_SquareVA;
	Fandango::Ref<Fandango::Shader> m_FlatColorShader;

	Fandango::Ref<Fandango::Texture2D> m_SpriteSheet, m_CheckerboardTexture;
	Fandango::Ref<Fandango::SubTexture2D> m_BarrelSubTexture, m_StairsSubTexture, m_TreeSubTexture;
	Fandango::Ref<Fandango::FrameBuffer> m_FrameBuffer;

	glm::vec4 m_SquareColor = { 0.2, 0.3f, 0.8f, 1.0f };
};