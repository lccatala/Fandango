#pragma once

#include "Fandango/Core/Window.h"
#include "Fandango/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Fandango
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& properties);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline uint32_t GetWidth() const override { return m_Data.Width; }
		inline uint32_t GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		
		inline virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowProperties& properties);
		virtual void ShutDown();

		GLFWwindow* m_Window;
		GraphicsContext *m_Context;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			bool Vsync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}