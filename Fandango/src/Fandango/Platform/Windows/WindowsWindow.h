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

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		
		inline virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void init(const WindowProperties& properties);
		virtual void shutDown();

		GLFWwindow* m_Window;
		GraphicsContext *m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool Vsync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}