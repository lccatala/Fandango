#include "fndgpch.h"
#include "Entity.h"

namespace Fandango
{

	Entity::Entity(entt::entity id, Scene* scene)
		: m_EntityID(id), m_Scene(scene)
	{

	}
}