#pragma once

#include "Fandango/Core/BaseDefines.h"
#include "Layer.h"

#include <fndgpch.h>

namespace Fandango
{
	class FNDG_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		bool PopLayer(Layer* layer);
		bool PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}

