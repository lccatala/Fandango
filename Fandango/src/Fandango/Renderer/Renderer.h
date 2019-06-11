#pragma once

namespace Fandango
{
	enum class RendererAPI
	{
		None = 0, OpenGL
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return s_RendererAPI; }
		inline static void SetAPI(const RendererAPI& api) { s_RendererAPI = api; }

	private:
		static RendererAPI s_RendererAPI;

	};

}