#pragma once

#include "Camera.h"
#include "OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"
#include "Fandango/Scene/Components.h"
#include "Fandango/Renderer/EditorCamera.h"

namespace Fandango
{
	class Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawQuadColor(const glm::vec2& position, float rotation, 
			const glm::vec2& size, const glm::vec4& color);
		static void DrawQuadColor(const glm::vec3& position, float rotation,
			const glm::vec2& size, const glm::vec4& color);

		static void DrawQuadSubtexture(const glm::vec2& position, float rotation,
			float tilingFactor, const glm::vec2& size, const Ref<SubTexture2D>& subtexture);
		static void DrawQuadSubtexture(const glm::vec3& position, float rotation,
			float tilingFactor, const glm::vec2& size, const Ref<SubTexture2D>& subtexture);

		static void DrawQuadTexture(const glm::vec2& position, float rotation,
			float tilingFactor, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuadTexture(const glm::vec3& position, float rotation,
			float tilingFactor, const glm::vec2& size, const Ref<Texture2D>& texture);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);

		struct RendererStats
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static void ResetStats();
		static RendererStats GetStats();

	private:
		static void StartNewBatch();
	};
}