#pragma once

namespace Lag
{
	class IRenderTargetListener
	{
	public:
		virtual ~IRenderTargetListener() {}

		virtual void onPreRender() = 0;
		virtual void onPostRender() = 0;
		virtual void onResize(int width, int height) = 0;
	};
}