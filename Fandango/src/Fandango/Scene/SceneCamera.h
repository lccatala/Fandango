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

		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType pt) { m_ProjectionType = pt; RecalculateProjection(); }

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }

		float GetOrthographicNearClip() const { return m_OrthographicNear; }
		void SetOrthographicNearClip(float nc) { m_OrthographicNear = nc; RecalculateProjection(); }

		float GetOrthographicFarClip() const { return m_OrthographicFar; }
		void SetOrthographicFarClip(float nc) { m_OrthographicFar = nc; RecalculateProjection(); }

		float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveFOV(float fov) { m_PerspectiveFOV = fov; RecalculateProjection(); }

		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(float nc) { m_PerspectiveNear = nc; RecalculateProjection(); }

		float GetPerspectiveFarClip() const { return m_PerspectiveNear; }
		void SetPerspectiveFarClip(float nc) { m_PerspectiveFar = nc; RecalculateProjection(); }
	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;
		void RecalculateProjection();

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar =  1.0f;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float m_AspectRatio = 0.0f;
	};
}