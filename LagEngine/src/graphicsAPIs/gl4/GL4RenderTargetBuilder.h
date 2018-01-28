#pragma once

#include "../../renderer/RenderTargetManager.h"
#include "GL4RenderToTexture.h"

namespace Lag
{	
	class GL4RenderTargetBuilder : public RenderTargetBuilder
	{
	public:
		virtual RenderTarget* build(const uint32 &name) const override
		{
			return new GL4RenderToTexture(width, height, renderPhase);
		}
	};
}