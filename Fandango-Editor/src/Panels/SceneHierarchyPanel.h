#pragma once

#include <Fandango.h>

namespace Fandango
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);
		Entity GetSelectedEntity() const { return m_SelectionContext; }

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawProperties(Entity entity);
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}