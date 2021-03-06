#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Fandango
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual std::string GetName() const = 0;

		virtual void SetUniform(const std::string& name, const glm::mat4& value) = 0;
		virtual void SetUniform(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetUniform(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;

		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		static Ref<Shader> Create(const std::string& filepath);
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader> &shader);
		Ref<Shader> Load(const std::string &filepath); // Stores as filename minus ".glsl"
		Ref<Shader> Load(const std::string& name, const std::string &filepath);

		Ref<Shader> Get(const std::string& name);
	private:
		bool Exists(const std::string& name);

		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}