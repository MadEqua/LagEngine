#pragma once

#include "../../core/IFactory.h"
#include "../../Types.h"

namespace Lag
{
	class IndexBuffer;

	class IndexBufferFactory : public IFactory<IndexBuffer>
	{
	public:
		IndexBufferFactory() {}
		virtual ~IndexBufferFactory() {}

		virtual IndexBuffer* create() const = 0;

		uint32 indexCount;
		uint32 indexSizeBytes;
		bool useMirrorBuffer;
	};
}