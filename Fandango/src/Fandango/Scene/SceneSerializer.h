#pragma once

#include "Scene.h"

namespace Fandango
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void SerializeText(const std::string& filepath);
		void SerializeBinary(const std::string& filepath);

		bool DeSerializeText(const std::string& filepath);
		bool DeSerializeBinary(const std::string& filepath);
	private:
		Ref<Scene> m_Scene;
	};
}