#pragma once

#include "../../renderer/graphicsAPI/VertexBuffer.h"

namespace Lag
{
	class GL4VertexBuffer :public VertexBuffer
	{
	public:
		GL4VertexBuffer(uint32 vertexCount, uint32 vertexSizeBytes, bool useMirror);
		virtual ~GL4VertexBuffer();

	protected:
		virtual void lockImplementation(uint32 offset, uint32 length) override;
		virtual void unlockImplementation() override;

		virtual byte* mapImplementation() override;
		virtual void unmapImplementation() override;

		virtual void writeImplementation(uint32 offset, uint32 length, byte* src) override;
		virtual void readImplementation(uint32 offset, uint32 length, byte* dst) override;

		virtual void bind() const override;
	};
}