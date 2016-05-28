#pragma once

namespace Lag
{
	class RenderTarget;
	
	class IRenderTargetListener
	{
	public:
		virtual ~IRenderTargetListener() {}

		virtual void onPreRender(RenderTarget &notifier) = 0;
		virtual void onPostRender(RenderTarget &notifier) = 0;
		virtual void onResize(RenderTarget &notifier, int width, int height) = 0;
	};
}