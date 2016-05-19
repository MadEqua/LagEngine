#pragma once

#include "../../renderer/graphicsAPI/IndexBuffer.h"

namespace Lag
{
	class GL4IndexBuffer : public IndexBuffer
	{
	public:
		GL4IndexBuffer(uint32 indexCount, uint32 indexSizeBytes, bool useMirror);
		virtual ~GL4IndexBuffer();

	protected:
		virtual void lockImplementation(uint32 offset, uint32 length) override;
		virtual void unlockImplementation() override;

		virtual byte* mapImplementation() override;
		virtual void unmapImplementation() override;

		virtual void writeImplementation(uint32 offset, uint32 length, byte* src) override;
		virtual void readImplementation(uint32 offset, uint32 length, byte* dst) override;
	};
}