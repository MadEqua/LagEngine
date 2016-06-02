#pragma once

#include <string>
#include "../../Types.h"

namespace Lag
{
	class GpuProgram;
	class Texture;
	enum RenderMode;
	enum BufferType;
	enum IndexType;
	class GpuBuffer;
	class InputDescription;
	class Color;
	
	/*
	* Abstracting the calls of a Graphics API. A class implementing this interface is used
	* to interact with the graphics pipeline and issue the efective draw calls.
	*/
	class IGraphicsAPI
	{
	public:
		IGraphicsAPI() {}
		virtual ~IGraphicsAPI() {}


		virtual void renderVertices(RenderMode mode, uint32 first, uint32 count) = 0;
		virtual void renderIndexed(RenderMode mode, uint32 first, IndexType indexType, uint32 count, uint32 baseVertex = 0) = 0;

		/*virtual void renderMultiVertices(RenderMode mode, uint32 first[], uint32 count[], uint32 drawCount) = 0;
		virtual void renderMultiIndexed(RenderMode mode, uint32 first[], uint32 count[], uint32 drawCount) = 0;

		virtual void renderVerticesInstanced(RenderMode mode, uint32 first, uint32 count, uint32 instanceCount) = 0;
		virtual void renderIndexedInstanced(RenderMode mode, uint32 first, uint32 count, uint32 instanceCount) = 0;*/

		virtual void clearColorBuffer(const Color &color) = 0;
		virtual void clearDepthBuffer(float value) = 0;
		virtual void clearStencilBuffer(int32 value) = 0;
		virtual void clearDepthAndStencilBuffer(float depth, int32 stencil) = 0;

		virtual void bindVertexBuffer(const GpuBuffer &vertexBuffer) = 0;
		virtual void bindIndexBuffer(const GpuBuffer &indexBuffer) = 0;
		virtual void bindGpuProgram(const GpuProgram &gpuProgram) = 0;
		virtual void bindInputDescription(const InputDescription &inputDescription) = 0;
		virtual void bindViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
		virtual void bindTexture(const Texture &texture, uint8 unit = 0) = 0;


		//virtual void setTexture(uint32 unit, Texture &texture) = 0;
		//virtual void setTexture(uint32 unit, const std::string &textureName) = 0;


		//virtual void setStencilCheckEnabled(bool enabled) = 0;
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