#pragma once

#include "Window.h"
#include "Fandango/LayerStack.h"
#include "Events/Event.h"
#include "Fandango/Events/ApplicationEvent.h"
#include "Fandango/DebugUI/DebugUILayer.h"
#include "Fandango/Renderer/Buffer.h"

#include "Fandango/Renderer/Shader.h"

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
		bool onWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		DebugUILayer* m_DebugUILayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();

}