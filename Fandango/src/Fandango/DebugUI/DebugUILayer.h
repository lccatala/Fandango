#pragma once

#include "Fandango/Core/Layer.h"
#include "Fandango/Events/MouseEvent.h"
#include "Fandango/Events/KeyEvent.h"
#include "Fandango/Events/ApplicationEvent.h"

namespace Fandango
{
	class FNDG_API DebugUILayer : public Layer
	{
	public:
		DebugUILayer();
		~DebugUILayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}