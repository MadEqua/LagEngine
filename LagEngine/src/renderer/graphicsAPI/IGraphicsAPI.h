#pragma once

#include <string>
#include "../../Types.h"

namespace Lag
{
	class GpuProgram;
	class Texture;
	class Viewport;
	
	class IGraphicsAPI
	{
	public:
		virtual ~IGraphicsAPI() {}

		virtual void setTexture(uint32 unit, Texture &texture) = 0;
		virtual void setTexture(uint32 unit, const std::string &textureName) = 0;

		virtual void setStencilCheckEnabled(bool enabled) = 0;
		/*virtual void setStencilBufferParams(CompareFunction func = CMPF_ALWAYS_PASS,
		uint32 refValue = 0, uint32 compareMask = 0xFFFFFFFF, uint32 writeMask = 0xFFFFFFFF,
		StencilOperation stencilFailOp = SOP_KEEP,
		StencilOperation depthFailOp = SOP_KEEP,
		StencilOperation passOp = SOP_KEEP,
		bool twoSidedOperation = false,
		bool readBackAsTexture = false) = 0;*/

		virtual void bindGpuProgram(GpuProgram &program) = 0;

		/*TODO: set clipping planes*/

		virtual void clearFrameBuffer(/*buffers, color?*/) = 0;
	};
}