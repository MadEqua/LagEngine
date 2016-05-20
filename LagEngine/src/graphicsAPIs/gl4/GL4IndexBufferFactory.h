#pragma once

#include "../../renderer/graphicsAPI/IndexBufferFactory.h"
#include "GL4IndexBuffer.h"

namespace Lag
{
	class GL4IndexBufferFactory : public IndexBufferFactory
	{
	public:
		GL4IndexBufferFactory() {}
		virtual ~GL4IndexBufferFactory() {}

		virtual GL4IndexBuffer* create() const { return new GL4IndexBuffer(indexCount, indexSizeBytes, useMirrorBuffer); }
	};
}
