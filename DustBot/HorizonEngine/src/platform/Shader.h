#pragma once

#include "core\ExportMacro.h"

#include <string>
#include <GL\glew.h>

namespace Horizon
{
	namespace Graphics 
	{
		class HORIZON_API Shader
		{
		public:
			friend class ShaderProgram;
			Shader(const GLenum& shaderType, const std::string& shaderSource);
			Shader(const GLenum& shaderType, const std::string& shaderSource, const std::string& fileName, const std::string& shaderPartName);
			~Shader();

			GLuint GetShaderId() const;

			static std::string GetShaderTypeName(const GLenum shaderType);

			static bool ValidateShaderCompilation(const GLuint& shaderId);

		protected:
			bool Reload();

		private:
			bool m_canReload;

			bool CreateShader(const std::string& shaderSource);
			bool CompileShader(const GLuint& shaderId);

			GLenum m_shaderType;
			std::string m_fileName;
			std::string m_shaderPartName;
			std::string m_shaderSource;
			GLuint m_shaderId;
		};
	}
}
