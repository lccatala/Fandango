#pragma once
#include "Fandango/Renderer/Shader.h"

#include <glm/glm.hpp>

namespace Fandango
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();
		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void UploadUniform(const std::string& name, int value);
		virtual void UploadUniform(const std::string& name, float value);
		virtual void UploadUniform(const std::string& name, const glm::mat4& matrix);
		virtual void UploadUniform(const std::string& name, const glm::mat3& matrix);
		virtual void UploadUniform(const std::string& name, const glm::vec4& vec);
		virtual void UploadUniform(const std::string& name, const glm::vec3& vec);
	private:
		int GetUniformLocation(const std::string& name) const;

		uint32_t m_RendererID;
		mutable std::unordered_map<std::string, int> m_UniformLocationCache;
	};
}