#pragma once
#include <GL\glew.h>
#include "platform\Texture.h"
#include "platform\structures\TextureData.h"

namespace Horizon
{
	namespace Graphics
	{
		class Texture2D : public Texture
		{
		public:
			Texture2D(const TextureData& texture);
			~Texture2D();
		};
	}
}

