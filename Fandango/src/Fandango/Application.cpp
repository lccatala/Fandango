#include "fndgpch.h"
#include "Application.h"

#include "Fandango/Log.h"
#include "Input.h"
#include "Fandango/Renderer/Renderer.h"

#include <GLFW/glfw3.h>


namespace Fandango {

#define BIND_EVENT_FUNCTION(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		FNDG_ENGINE_ASSERT(s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNCTION(OnEvent));

		m_DebugUILayer = new DebugUILayer();
		PushOverlay(m_DebugUILayer);
	}


	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			// Move this lines into a "Platform" class
			float time = (float)glfwGetTime();

			TimeStep ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(ts);

			m_DebugUILayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_DebugUILayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e) 
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.IsHandled())
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}