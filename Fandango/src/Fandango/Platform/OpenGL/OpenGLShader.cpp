#include "fndgpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

namespace Fandango
{
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		FNDG_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		FNDG_PROFILE_FUNCTION();

		std::string contents = ReadFile(filepath);
		auto shaderSources = PreProcess(contents);
		Compile(shaderSources);
		m_Name = ExtractName(filepath);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		FNDG_PROFILE_FUNCTION();

		std::string result;
		std::ifstream file(filepath, std::ios::in | std::ios::binary);
		if (file)
		{
			file.seekg(0, std::ios::end); // Pointing at the end of the file
			result.resize(file.tellg()); // Resize output string to file size
			file.seekg(0, std::ios::beg); // Move to the beggining of the file
			file.read(&result[0], result.size());
			file.close();
		}
		else
		{
			FNDG_ENGINE_ERROR("Could not open file {0}", filepath);
		}
		return result;
	}

	OpenGLShader::~OpenGLShader()
	{
		FNDG_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		FNDG_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const
	{
		FNDG_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::mat4& value)
	{
		FNDG_PROFILE_FUNCTION();

		UploadUniform(name, value);
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::vec3& value)
	{
		FNDG_PROFILE_FUNCTION();

		UploadUniform(name, value);
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::vec4& value)
	{
		FNDG_PROFILE_FUNCTION();

		UploadUniform(name, value);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		FNDG_PROFILE_FUNCTION();

		UploadUniform(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		UploadUniform(name, values, count);
	}

	GLint OpenGLShader::GetUniformLocation(const std::string& name) const
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		m_UniformLocationCache[name] = location;
		return location;
	}

	void OpenGLShader::UploadUniform(const std::string& name, int value)
	{
		GLint location = GetUniformLocation(name);
		glUniform1i(location, value);
	}
	
	void OpenGLShader::UploadUniform(const std::string& name, int* values, uint32_t count)
	{
		GLint location = GetUniformLocation(name);
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniform(const std::string& name, float value)
	{
		GLint location = GetUniformLocation(name);
		glUniform1f(location, value);
	}
	
	void OpenGLShader::UploadUniform(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = GetUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	
	void OpenGLShader::UploadUniform(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = GetUniformLocation(name);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniform(const std::string& name, const glm::vec4& vec)
	{
		GLint location = GetUniformLocation(name);
		glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
	}
	
	void OpenGLShader::UploadUniform(const std::string& name, const glm::vec3& vec)
	{
		GLint location = GetUniformLocation(name);
		glUniform3f(location, vec.x, vec.y, vec.z);
	}

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		FNDG_ENGINE_ASSERT(false, "Unknown shader type: {0}", type);
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& src)
	{
		std::unordered_map<GLenum, std::string> sources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = src.find(typeToken, 0); // Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = src.find_first_of("\r\n", pos); // End of shader type declaration line
			FNDG_ENGINE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1; // Start of shader type name
			std::string type = src.substr(begin, eol - begin);
			FNDG_ENGINE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specification");

			size_t nextLinePos = src.find_first_not_of("\r\n", eol);
			FNDG_ENGINE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = src.find(typeToken, nextLinePos);

			if (pos == std::string::npos)
				sources[ShaderTypeFromString(type)] = src.substr(nextLinePos);
			else
				sources[ShaderTypeFromString(type)] = src.substr(nextLinePos, pos - nextLinePos);


			//sources[ShaderTypeFromString(type)] = src.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? src.size() - 1 : nextLinePos));
		}

		return sources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string> sources)
	{
		GLuint program = glCreateProgram();
		FNDG_ENGINE_ASSERT(sources.size() <= 2, "Too many shaders, max ammount is 2");
		std::array<GLenum, 2> glShaderIDs;

		int shaderIndex = 0;

		for (auto& kv : sources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;
			GLuint shader = glCreateShader(type);

			const char* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			int isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				int maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				FNDG_ENGINE_ERROR("{0}", infoLog.data());
				FNDG_ENGINE_ASSERT(false, "Shader compilation failure");
				break;
			}
			glAttachShader(program, shader);
			glShaderIDs[shaderIndex++] = shader;
		}

		m_RendererID = program;

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		int isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)& isLinked);
		if (isLinked == GL_FALSE)
		{
			int maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIDs)
				glDeleteShader(id);

			FNDG_ENGINE_ERROR("{0}", infoLog.data());
			FNDG_ENGINE_ASSERT(false, "Shader linking failure");
			return;
		}

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
	}

	std::string OpenGLShader::ExtractName(const std::string& filepath)
	{
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind(".");

		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		return filepath.substr(lastSlash, count);
	}
}