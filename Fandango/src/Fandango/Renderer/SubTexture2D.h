#pragma once

#include <Fandango/Renderer/Texture.h>
#include <glm/glm.hpp>

namespace Fandango
{
	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, glm::vec2 min, glm::vec2 max);
		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D> texture, const glm::vec2& pos, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1, 1 });

		const glm::vec2* GetTexCoords() { return m_TexCoords; }
		const Ref<Texture2D> GetTexture() { return m_Texture; }
	private:
		glm::vec2 m_TexCoords[4];
		Ref<Texture2D> m_Texture;
	};
}