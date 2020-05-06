#pragma once

#include "Fandango/Core/TimeStep.h"
#include "Fandango/Renderer/Camera.h"
#include "Fandango/Events/MouseEvent.h"
#include "Fandango/Events/ApplicationEvent.h"

namespace Fandango
{
	struct OrthographicCameraBounds
	{
		float Left, Right, Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
		const OrthographicCameraBounds& GetCameraBounds() const { return m_CameraBounds; }
	private:
		bool OnMouseScrolled(MouseWheelScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCameraBounds m_CameraBounds;
		OrthographicCamera m_Camera;

		bool m_Rotation;
		float m_CameraRotation = 0.0f;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };

		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.f;
	};
}