#pragma once

#include "fndgpch.h"

#include "Fandango/Core/Core.h"
#include "Fandango/Events/Event.h"

namespace Fandango
{
	struct WindowProperties
	{
		std::string m_Title;
		uint32_t m_Width;
		uint32_t m_Height;

		WindowProperties(const std::string& title = "Fandango App", uint32_t width = 1280, uint32_t height = 720)
			: m_Title(title), m_Width(width), m_Height(height)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		// Since this class is platform independent, we'll implement a create() 
		// method per platform instead of having a universal constructor
		static Window* Create(const WindowProperties& properties = WindowProperties());
	};
}