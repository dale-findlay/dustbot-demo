#include "Shader.h"
#include "GLHelper.h"
#include "subsystems\VirtualFileSystem.h"
#include "utilities\IOUtility.h"
#include "utilities\ShaderIOUtility.h"

Horizon::Graphics::Shader::Shader(const GLenum & shaderType, const std::string & shaderSource)
	: m_shaderId(-1), m_canReload(false)
{
	m_shaderType = shaderType;

	CreateShader(shaderSource);
}

Horizon::Graphics::Shader::Shader(const GLenum& shaderType, const std::string& shaderSource, const std::string& fileName, const std::string& shaderPartName)
	: m_shaderId(-1)
{
	m_fileName = fileName;
	m_shaderType = shaderType;
	m_shaderPartName = shaderPartName;

	CreateShader(shaderSource);
}

Horizon::Graphics::Shader::~Shader()
{
	if (m_shaderId != -1)
	{
		GL(glDeleteShader(m_shaderId));
	}
}

GLuint Horizon::Graphics::Shader::GetShaderId() const
{
	return m_shaderId;
}

std::string Horizon::Graphics::Shader::GetShaderTypeName(const GLenum shaderType)
{
	std::string shaderTypeString = "UNKNOWN_TYPE";

	switch (shaderType)
	{
	case GL_VERTEX_SHADER:
	{
		shaderTypeString = "GL_VERTEX_SHADER";
		break;
	}
	case GL_TESS_CONTROL_SHADER:
	{
		shaderTypeString = "GL_TESS_CONTROL_SHADER";
		break;
	}
	case GL_TESS_EVALUATION_SHADER:
	{
		shaderTypeString = "GL_TESS_EVALUATION_SHADER";
		break;
	}
	case GL_GEOMETRY_SHADER:
	{
		shaderTypeString = "GL_GEOMETRY_SHADER";
		break;
	}
	case GL_FRAGMENT_SHADER:
	{
		shaderTypeString = "GL_FRAGMENT_SHADER";
		break;
	}
	}

	return shaderTypeString;
}

bool Horizon::Graphics::Shader::CreateShader(const std::string& shaderSource)
{
	GLuint newShaderId = -1;
	GL(newShaderId = glCreateShader(m_shaderType));

	const char* shaderSourceCString = shaderSource.c_str();
	GL(glShaderSource(newShaderId, 1, &shaderSourceCString, NULL));
	if (!CompileShader(newShaderId))
	{
		Logger::GetInstance()->LogError("Failed to compile shader id: " + std::to_string(m_shaderId) + " with shader source " + shaderSource);
		GL(glDeleteShader(newShaderId));
		return false;
	}

	//Delete old shader (reloading)
	if (m_shaderId != -1)
	{
		GL(glDeleteShader(m_shaderId));
	}

	m_shaderId = newShaderId;
	return true;

}

bool Horizon::Graphics::Shader::CompileShader(const GLuint& shaderId)
{
	GL(glCompileShader(shaderId));
	return ValidateShaderCompilation(shaderId);
}

bool Horizon::Graphics::Shader::ValidateShaderCompilation(const GLuint& shaderId)
{
	GLint success;
	GLchar infoLog[512];

	GL(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GL(glGetShaderInfoLog(shaderId, 512, NULL, infoLog));
		Logger::GetInstance()->LogError("Failed to Compile Shader " + std::to_string(shaderId) + " error: " + std::string(infoLog));
	}

	return (bool)success;
}

bool Horizon::Graphics::Shader::Reload()
{
	if (!m_canReload)
	{
		//Logger::GetInstance()->LogWarning("Shader with id " + std::to_string(m_shaderId) + " could not be reloaded.");
		return false;
	}

	auto shaderParts = ShaderIOUtility::GetShaderParts(m_fileName);

	std::string shaderSource = shaderParts[m_shaderPartName];

	bool createShader = CreateShader(shaderSource);

	return true;
}
