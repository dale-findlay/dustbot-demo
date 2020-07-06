#include "Texture2D.h"

#include "platform\GLHelper.h"

Horizon::Graphics::Texture2D::Texture2D(const TextureData& texture)
	: Texture(GL_TEXTURE_2D)
{
	Bind();
	GL(glTexImage2D(
		GetTarget(),				//target
		0,							//level
		texture.internalFormat,		//internal format
		texture.textureWidth,		//width
		texture.textureHeight,		//height
		0,							//border
		texture.textureDataFormat,	//format
		texture.textureDataType,	//type
		texture.textureData			//data
	));
	GL(glGenerateMipmap(GL_TEXTURE_2D));
	Unbind();
}

Horizon::Graphics::Texture2D::~Texture2D()
{
}
 