#pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"
#include "platform\Shader.h"
#include "platform\structures\VertexAttribute.h"

#include <string>
#include <vector>
#include <GL\glew.h>
#include <glm\common.hpp>

namespace Horizon
{
	namespace Graphics
	{
		class HORIZON_API ShaderProgram
		{
		public:
			ShaderProgram();
			~ShaderProgram();

			void QueueReload();

			void Use();

			void BindAttribLocation(const unsigned int& index, const std::string& name);

			void AttachShader(Shader* shader);
			bool LinkShaderProgram();

			static bool ValidateShaderLinkage(const GLuint& shaderProgramId);

			void BindUniform(const std::string& uniformName, const float& f);
			void BindUniform(const std::string& uniformName, const int& i);
			void BindUniform(const std::string& uniformName, const unsigned int& ui);

			void BindUniform(const std::string& uniformName, const glm::vec2& vector);
			void BindUniform(const std::string& uniformName, const glm::vec3& vector);
			void BindUniform(const std::string& uniformName, const glm::vec4& vector);

			void BindUniform(const std::string& uniformName, const glm::mat2& matrix);
			void BindUniform(const std::string& uniformName, const glm::mat3& matrix);
			void BindUniform(const std::string& uniformName, const glm::mat4& matrix);

		private:
			bool ReloadShaderProgram();

			GLuint m_shaderProgramId;
			bool m_reload;

			std::vector<Shader*> m_shaders;
			std::vector<VertexAttribute> m_vertexAttributes;

		};
	}
}
