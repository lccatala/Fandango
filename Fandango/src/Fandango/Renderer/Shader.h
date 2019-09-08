#pragma once

#include <string>
#include <unordered_map>

namespace Fandango
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual std::string GetName() const = 0;

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