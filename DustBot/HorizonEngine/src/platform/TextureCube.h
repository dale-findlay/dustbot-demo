#pragma once


#include <GL\glew.h>
#include <vector>

#include "platform\Texture.h"
#include "platform\structures\TextureData.h"

namespace Horizon
{
	namespace Graphics
	{
		class TextureCube : public Texture
		{
		public:
			TextureCube(const std::vector<TextureData> textures);
			~TextureCube();
		};
	}
}
