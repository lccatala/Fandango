#pragma once

#include "Fandango/Core/BaseDefines.h"
#include "Fandango/Events/Event.h"
#include "Fandango/Core/TimeStep.h"

namespace Fandango
{
	class FNDG_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_DebugName; }
		
	protected:

		// Use this only during debugging, since you shouldn't perform 
		// operations like searching for layers based on their name
		std::string m_DebugName;
	};
}

