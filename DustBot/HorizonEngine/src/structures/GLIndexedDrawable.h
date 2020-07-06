 #pragma once

#include "core\ExportMacro.h"
#include "core\Material.h"
#include "platform\VertexArray.h"
#include "platform\VertexBuffer.h"
#include "platform\IndexBuffer.h"

namespace Horizon
{
	struct HORIZON_API  GLIndexedDrawable
	{
		Material* material;
		Graphics::VertexArray* vertexArray;
		Graphics::VertexBuffer* vertexBuffer;
		Graphics::IndexBuffer* indexBuffer;
	};
}
