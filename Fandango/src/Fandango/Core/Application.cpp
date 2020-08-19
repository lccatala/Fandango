#include "fndgpch.h"
#include "Application.h"

#include "Fandango/Core/Log.h"
#include "Input.h"
#include "Fandango/Renderer/Renderer.h"

#include <GLFW/glfw3.h>


namespace Fandango {

#define BIND_EVENT_FUNCTION(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		Application("Fandango App");
	}

	Application::Application(const std::string& name)
	{
		FNDG_PROFILE_FUNCTION();
		FNDG_ENGINE_ASSERT(s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProperties(name)));
		m_Window->SetEventCallback(BIND_EVENT_FUNCTION(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}


	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		FNDG_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		FNDG_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Run()
	{
		FNDG_PROFILE_FUNCTION();

		while (m_Running)
		{
			FNDG_PROFILE_SCOPE("RunLoop");
			// Move this line into a "Platform" class
			float time = (float)glfwGetTime();

			TimeStep ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					FNDG_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(ts);
					m_ImGuiLayer->Begin();
				}

				{
					FNDG_PROFILE_SCOPE("LayerStack OnImGuiRender");
						for (Layer* layer : m_LayerStack)
							layer->OnImGuiRender();
						m_ImGuiLayer->End();
				}
			}

			m_Window->OnUpdate();
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e) 
	{
		FNDG_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(OnWindowResize));

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
	
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		FNDG_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}