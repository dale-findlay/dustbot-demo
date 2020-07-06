#pragma once

#include "core\TypeAbstractions.h"
#include "core\ExportMacro.h"
#include "utilities\Logger.h"

#include <GL\glew.h>

namespace Horizon
{
	namespace Graphics
	{
		struct HORIZON_API  VertexBufferLayoutElement
		{
			uint32 count;
			uint32 type;
			unsigned char normalized;
			size_t offset;

			static uint32 GetGLTypeSize(const uint32 glType)
			{
				int32 glTypeSize = 0;
				switch (glType)
				{
				case GL_FLOAT: {glTypeSize = 4; break; }
				case GL_UNSIGNED_BYTE: {glTypeSize = 1; break; }
				case GL_UNSIGNED_INT: {glTypeSize = 4; break; }
				default: {Horizon::Logger::GetInstance()->LogError("Unknown GL Type " + std::to_string(glType)); break; }
				}

				return glTypeSize;
			}
		};
	}
}