#pragma once

#include "../../core/IFactory.h"
#include "../../Types.h"

namespace Lag
{
	class VertexBuffer;
	
	class VertexBufferFactory : public IFactory<VertexBuffer>
	{
	public:
		VertexBufferFactory() {}
		virtual ~VertexBufferFactory() {}

		virtual VertexBuffer* create() const = 0;

		uint32 vertexCount;
		uint32 vertexSizeBytes;
		bool useMirrorBuffer;
	};
}