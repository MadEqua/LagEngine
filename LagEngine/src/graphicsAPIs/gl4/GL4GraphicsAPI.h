#pragma once

#include "../../Renderer/graphicsApi/IGraphicsApi.h"

namespace Lag
{
	class GL4GraphicsAPI : public IGraphicsAPI
	{
	public:
		GL4GraphicsAPI();
		~GL4GraphicsAPI();

		virtual void renderVertices();
		virtual void renderIndexed();

		virtual void setTexture(uint32 unit, Texture & texture);
		virtual void setTexture(uint32 unit, const std::string & textureName);
		virtual void setStencilCheckEnabled(bool enabled);
		virtual void bindGpuProgram(GpuProgram & program);
		virtual void clearFrameBuffer();
	};
}