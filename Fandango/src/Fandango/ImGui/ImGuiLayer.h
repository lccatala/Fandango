#pragma once

#include "Fandango/Core/Layer.h"
#include "Fandango/Events/MouseEvent.h"
#include "Fandango/Events/KeyEvent.h"
#include "Fandango/Events/ApplicationEvent.h"

namespace Fandango
{
	class FNDG_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();
		void SetBlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();
	private:
		float m_Time = 0.0f;
		bool m_BlockEvents = true;
	};
}