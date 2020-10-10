#pragma once

#include "Fandango/Core/TimeStep.h"
#include "Fandango/Renderer/Camera.h"
#include "Fandango/Renderer/OrthographicCamera.h"
#include "Fandango/Events/MouseEvent.h"
#include "Fandango/Events/ApplicationEvent.h"

namespace Fandango
{

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		void Resize(float width, float height);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseWheelScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;
		float m_CameraRotation = 0.0f;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };

		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.f;
	};
}