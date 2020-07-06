#include "ShaderProgram.h"

#include "platform\GLHelper.h"

#include <glm\gtc\matrix_transform.hpp>

Horizon::Graphics::ShaderProgram::ShaderProgram()
{
	GL(m_shaderProgramId = glCreateProgram());
}

Horizon::Graphics::ShaderProgram::~ShaderProgram()
{
	GL(glDeleteProgram(m_shaderProgramId));
}

void Horizon::Graphics::ShaderProgram::QueueReload()
{
	m_reload = true;
}

void Horizon::Graphics::ShaderProgram::Use()
{
	if (m_reload)
	{
		ReloadShaderProgram();
		m_reload = false;
	}

	GL(glUseProgram(m_shaderProgramId));
}

void Horizon::Graphics::ShaderProgram::BindAttribLocation(const unsigned int& index, const std::string& name)
{
	m_vertexAttributes.emplace_back(index, name);
	GL(glBindAttribLocation(m_shaderProgramId, index, name.c_str()));
}

void Horizon::Graphics::ShaderProgram::AttachShader(Shader* shader)
{
	m_shaders.push_back(shader);
		GL(glAttachShader(m_shaderProgramId, shader->GetShaderId()));
}

bool Horizon::Graphics::ShaderProgram::LinkShaderProgram()
{
	GL(glLinkProgram(m_shaderProgramId));
	return ValidateShaderLinkage(m_shaderProgramId);
}

bool Horizon::Graphics::ShaderProgram::ValidateShaderLinkage(const GLuint& shaderProgramId)
{
	GLint success;
	GLchar infoLog[512];

	GL(glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success));
	if (!success)
	{
		GL(glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog));
		Logger::GetInstance()->LogError("Failed to Link Shader Program. Id:" + std::to_string(shaderProgramId) + " error: " + std::string(infoLog));
	}

	return success;
}

void Horizon::Graphics::ShaderProgram::BindUniform(const std::string& uniformName, const float& f)
{
	
	GL(GLint uniformLocation = glGetUniformLocation(m_shaderProgramId, uniformName.c_str()));
	GL(glUniform1f(uniformLocation, f));
}

void Horizon::Graphics::ShaderProgram::BindUniform(const std::string& uniformName, const int& i)
{
	
	GL(GLint uniformLocation = glGetUniformLocation(m_shaderProgramId, uniformName.c_str()));
	GL(glUniform1i(uniformLocation, i));
}

void Horizon::Graphics::ShaderProgram::BindUniform(const std::string& uniformName, const unsigned int& ui)
{
	
	GL(GLint uniformLocation = glGetUniformLocation(m_shaderProgramId, uniformName.c_str()));
	GL(glUniform1ui(uniformLocation, ui));
}

void Horizon::Graphics::ShaderProgram::BindUniform(const std::string& uniformName, const glm::vec2& vector)
{
	
	GL(GLint uniformLocation = glGetUniformLocation(m_shaderProgramId, uniformName.c_str()));
	GL(glUniform2fv(uniformLocation, 1, &vector[0]));
}

void Horizon::Graphics::ShaderProgram::BindUniform(const std::string& uniformName, const glm::vec3& vector)
{	
	GL(GLint uniformLocation = glGetUniformLocation(m_shaderProgramId, uniformName.c_str()));
	GL(glUniform3fv(uniformLocation, 1, &vector[0]));
}

void Horizon::Graphics::ShaderProgram::BindUniform(const std::string& uniformName, const glm::vec4& vector)
{
	
	GL(GLint uniformLocation = glGetUniformLocation(m_shaderProgramId, uniformName.c_str()));
	GL(glUniform4fv(uniformLocation, 1, &vector[0]));
}

void Horizon::Graphics::ShaderProgram::BindUniform(const std::string& uniformName, const glm::mat2& matrix)
{
	
	GL(GLint uniformLocation = glGetUniformLocation(m_shaderProgramId, uniformName.c_str()));
	GL(glUniformMatrix2fv(uniformLocation, 1, GL_FALSE, &matrix[0][0]));
}

void Horizon::Graphics::ShaderProgram::BindUniform(const std::string& uniformName, const glm::mat3& matrix)
{
	
	GL(GLint uniformLocation = glGetUniformLocation(m_shaderProgramId, uniformName.c_str()));
	GL(glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, &matrix[0][0]));
}

void Horizon::Graphics::ShaderProgram::BindUniform(const std::string& uniformName, const glm::mat4& matrix)
{
	
	GL(GLint uniformLocation = glGetUniformLocation(m_shaderProgramId, uniformName.c_str()));
	GL(glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &matrix[0][0]));
}

bool Horizon::Graphics::ShaderProgram::ReloadShaderProgram()
{
	GLuint newShaderProgram = -1;
	GL(newShaderProgram = glCreateProgram());

	bool success = false;

	int successfulShaderReload = 0;
	for (int i = 0; i < m_shaders.size(); ++i)
	{
		Shader* shader = m_shaders[i];
		bool result = shader->Reload();
		if (result == true)
		{
			GL(glAttachShader(newShaderProgram, shader->GetShaderId()));
			if (result == true)
			{
				successfulShaderReload++;
			}
		}
	}

	GL(glLinkProgram(newShaderProgram));
	bool linkResult = ValidateShaderLinkage(newShaderProgram);

	success = successfulShaderReload == m_shaders.size() && linkResult == true;

	if (success)
	{
		GLuint oldId = m_shaderProgramId;
		GL(glDeleteProgram(m_shaderProgramId));

		m_shaderProgramId = newShaderProgram;

		for (int i = 0; i < m_vertexAttributes.size(); ++i)
		{
			GL(glBindAttribLocation(m_shaderProgramId, m_vertexAttributes[i].index, m_vertexAttributes[i].name.c_str()));
		}
		Logger::GetInstance()->LogWarning("Shader " + std::to_string(oldId) + " Reloaded.");
	}

	if (!success)
	{
		GL(glDeleteProgram(newShaderProgram));
	}

	return success;
}
