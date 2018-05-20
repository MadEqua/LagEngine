#pragma once

#include "../../resources/RenderTargetManager.h"
#include "GLFWRenderWindow.h"

namespace Lag
{	
	class GLFWRenderTargetBuilder : public RenderTargetBuilder
	{
	public:
		virtual RenderTarget* build(const uint32 &name) const override
		{
			return new GLFWRenderWindow(initializationParameters);
		}
	};
}