#include "TextureCube.h"
#include <assert.h>


Horizon::Graphics::TextureCube::TextureCube(const std::vector<TextureData> textures)
	: Texture(GL_TEXTURE_CUBE_MAP)
{
	assert(textures.size() == 6);
	
	Bind();
	
	GLuint sideTargets[6] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X ,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X ,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y ,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y ,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z ,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (int i = 0; i < 6; ++i)
	{
		glTexImage2D(
			sideTargets[i],
			0,
			GL_RGBA,
			textures[i].textureWidth,
			textures[i].textureHeight,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			textures[i].textureData);
	}

	SetTextureParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SetTextureParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	SetTextureParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	SetTextureParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	SetTextureParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Horizon::Graphics::TextureCube::~TextureCube()
{}
