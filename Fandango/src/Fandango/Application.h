#pragma once

#include "Fandango/Core.h"

#include "Fandango/Core/TimeStep.h"
#include "Fandango/DebugUI/DebugUILayer.h"

#include "Fandango/Events/Event.h"
#include "Fandango/Events/ApplicationEvent.h"

#include "Fandango/LayerStack.h"

#include "Fandango/Renderer/Buffer.h"
#include "Fandango/Renderer/Camera.h"
#include "Fandango/Renderer/VertexArray.h"
#include "Fandango/Renderer/Shader.h"

#include "Fandango/Window.h"

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

		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();

}