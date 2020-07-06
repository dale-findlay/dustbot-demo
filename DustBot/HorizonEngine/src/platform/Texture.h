#pragma once

#include "core\ExportMacro.h"

#include <GL\glew.h>

namespace Horizon
{
	namespace Graphics
	{
		class HORIZON_API Texture
		{
		public:
			Texture(const GLenum target);
			~Texture();

			void Bind() const;
			void Unbind() const;

			void SetTextureParameter(const GLenum name, const GLint value);

			GLuint GetId() const;
			GLenum GetTarget() const;

		private:
			const GLenum m_target;
			GLuint m_glId;

		};
	}
}
