#pragma once

#include "core\ExportMacro.h"

#include <GL\glew.h>

namespace Horizon
{
	namespace Graphics
	{
		struct HORIZON_API  TextureData
		{
			unsigned char* textureData;
			int internalFormat;
			int textureWidth;
			int textureHeight;
			GLenum textureDataFormat;
			GLenum textureDataType;

		};
	}
}
