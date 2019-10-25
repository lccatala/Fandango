#pragma once
#include "Fandango/Renderer/Shader.h"

#include <glm/glm.hpp>

// TODO: remove!!
typedef unsigned int GLenum;

namespace Fandango
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& filepath);
		virtual ~OpenGLShader();
		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void SetUniform(const std::string& name, const glm::mat4& value) override;
		virtual void SetUniform(const std::string& name, const glm::vec3& value) override;
		virtual void SetUniform(const std::string& name, const glm::vec4& value) override;

		virtual std::string GetName() const override { return m_Name; }

		virtual void UploadUniform(const std::string& name, int value);
		virtual void UploadUniform(const std::string& name, float value);
		virtual void UploadUniform(const std::string& name, const glm::mat4& matrix);
		virtual void UploadUniform(const std::string& name, const glm::mat3& matrix);
		virtual void UploadUniform(const std::string& name, const glm::vec4& vec);
		virtual void UploadUniform(const std::string& name, const glm::vec3& vec);
	private:
		int GetUniformLocation(const std::string& name) const;
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& src);
		void Compile(const std::unordered_map<GLenum, std::string> sources);
		std::string ReadFile(const std::string& filepath);
		std::string ExtractName(const std::string& filepath);

		uint32_t m_RendererID;
		std::string m_Name;
		mutable std::unordered_map<std::string, int> m_UniformLocationCache;
	};
}