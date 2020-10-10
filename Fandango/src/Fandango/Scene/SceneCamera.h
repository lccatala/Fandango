#pragma once

#include <Fandango/Renderer/Camera.h>

#include <glm/glm.hpp>

namespace Fandango
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);
	private:
		void RecalculateProjection();

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar =  1.0f;
		float m_AspectRatio = 0.0f;
	};
}