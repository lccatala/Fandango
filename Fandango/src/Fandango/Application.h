#pragma once

#include "Core.h"

#include "Window.h"
#include "Fandango/LayerStack.h"

#include "Fandango/Events/Event.h"
#include "Fandango/Events/ApplicationEvent.h"
#include "Fandango/DebugUI/DebugUILayer.h"

#include "Fandango/Renderer/Buffer.h"
#include "Fandango/Renderer/VertexArray.h"
#include "Fandango/Renderer/Shader.h"
#include "Fandango/Renderer/Camera.h"


namespace Fandango {

	class FNDG_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static inline Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		DebugUILayer* m_DebugUILayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_TriangleShader;
		std::shared_ptr<VertexArray> m_TriangleVA;
		std::shared_ptr<Shader> m_SquareShader;
		std::shared_ptr<VertexArray> m_SquareVA;

		OrthographicCamera m_Camera;

		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();

}