#pragma once

#include "IRenderTargetListener.h"

namespace Lag
{
	class IWindowListener : public IRenderTargetListener
	{
	public:
		virtual ~IWindowListener() {}

		//NO WORK WITH MACROS OF OBSERVER :(
		/*virtual void onMove(RenderWindow &window, int x, int y) = 0;
		virtual void onResize(RenderWindow &window, int width, int height) = 0;
		virtual void onClose(RenderWindow &window) = 0;
		virtual void onFocusChange(RenderWindow &window, bool focused) = 0;*/

		virtual void onMove(int x, int y) = 0;
		virtual void onClose() = 0;
		virtual void onFocusChange(bool focused) = 0;
	};
}