#pragma once

#include <string>
#include <glm/glm.hpp>
namespace Fandango
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~Shader();

		virtual void Bind() const;
		virtual void UnBind() const;

		virtual void UploadUniform(const std::string& name, const glm::mat4& matrix);
	private:
		uint32_t m_RendererID;
	};
}