#pragma once

namespace Lag
{
	class RenderTarget;
	
	class IRenderTargetListener
	{
	public:
		virtual ~IRenderTargetListener() = default;

		virtual void onPreRender(RenderTarget &notifier) = 0;
		virtual void onPostRender(RenderTarget &notifier) = 0;
		virtual void onResize(RenderTarget &notifier, uint32 width, uint32 height) = 0;
	};
}