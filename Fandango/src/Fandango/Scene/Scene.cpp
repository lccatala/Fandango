#include "fndgpch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Fandango/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Fandango
{
	Scene::Scene()
	{
		
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::OnUpdate(TimeStep ts)
	{
		//Renderer2D::DrawQuadColor({ 0.0f, 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f });

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group) 
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}

}