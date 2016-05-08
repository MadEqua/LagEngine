#pragma once

namespace Lag
{
	class IRenderTarget
	{
	public:
		virtual ~IRenderTarget() {}

		//TODO
		virtual bool initialize() = 0;
		virtual void destroy() = 0;

		virtual void swapBuffers() = 0;
	};
}