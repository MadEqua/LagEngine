#pragma once

#include "GL4VertexBuffer.h"
#include "../../renderer/graphicsAPI/VertexBufferFactory.h"

namespace Lag
{
	class GL4VertexBufferFactory : public VertexBufferFactory
	{
	public:
		GL4VertexBufferFactory() {}
		virtual ~GL4VertexBufferFactory() {}

		virtual GL4VertexBuffer* create() const { return new GL4VertexBuffer(vertexCount, vertexSizeBytes, useMirrorBuffer); }
	};
}