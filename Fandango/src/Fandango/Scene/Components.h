#pragma once

#include <glm/glm.hpp>
#include "Fandango/Scene/SceneCamera.h"
#include "ScriptableEntity.h"

namespace Fandango
{
	struct CameraComponent
	{
		SceneCamera Camera;

		// Is this the scene's primary camera?
		// Scenes can have multiple cameras and only one will be set as primary
		bool Primary = true; // TODO: maybe move this logic to Scene

		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyInstanceScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyInstanceScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct TagComponent
	{
		std::string Tag;
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}

		// Instances of TransformComponent to return this->Transform
		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};
}
