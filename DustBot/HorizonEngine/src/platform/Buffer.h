#pragma once

#include "core\ExportMacro.h"

#include <GL\glew.h>

namespace Horizon
{
	namespace Graphics
	{
		class HORIZON_API Buffer
		{
		public:
			Buffer(GLenum target);
			~Buffer();

			void Bind() const;
			void Unbind() const;
		private:
			GLenum m_target;

		protected:
			GLuint m_glId;
		};
	}
}