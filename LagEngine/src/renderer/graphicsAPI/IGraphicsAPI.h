#pragma once

#include <string>
#include "../../Types.h"

namespace Lag
{
	class GpuProgram;
	class Texture;
	
	/*
	* Abstracting the calls of a Graphics API. A class implementing this interface is used
	* to interact with the graphics pipeline and issue the efective draw calls.
	*/
	class IGraphicsAPI
	{
	public:
		IGraphicsAPI() {}
		virtual ~IGraphicsAPI() {}

		virtual void renderVertices() = 0;
		virtual void renderIndexed() = 0;

		//TODO: all draw calls

		virtual void clearFrameBuffer(/*buffers, color?*/) = 0;

		virtual void setTexture(uint32 unit, Texture &texture) = 0;
		virtual void setTexture(uint32 unit, const std::string &textureName) = 0;

		//virtual void bindGpuProgram(GpuProgram &program) = 0;

		virtual void setStencilCheckEnabled(bool enabled) = 0;
		/*virtual void setStencilBufferParams(CompareFunction func = CMPF_ALWAYS_PASS,
		uint32 refValue = 0, uint32 compareMask = 0xFFFFFFFF, uint32 writeMask = 0xFFFFFFFF,
		StencilOperation stencilFailOp = SOP_KEEP,
		StencilOperation depthFailOp = SOP_KEEP,
		StencilOperation passOp = SOP_KEEP,
		bool twoSidedOperation = false,
		bool readBackAsTexture = false) = 0;*/


		/*TODO: set clipping planes*/

	};
}