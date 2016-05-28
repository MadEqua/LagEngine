#pragma once

#include "IRenderTargetListener.h"

namespace Lag
{
	class RenderWindow;
	
	class IWindowListener : public IRenderTargetListener
	{
	public:
		virtual ~IWindowListener() {}

		//NO WORK WITH MACROS OF OBSERVER :(
		/*virtual void onMove(RenderWindow &window, int x, int y) = 0;
		virtual void onResize(RenderWindow &window, int width, int height) = 0;
		virtual void onClose(RenderWindow &window) = 0;
		virtual void onFocusChange(RenderWindow &window, bool focused) = 0;*/

		virtual void onMove(RenderWindow &notifier, int x, int y) = 0;
		virtual void onClose(RenderWindow &notifier) = 0;
		virtual void onFocusChange(RenderWindow &notifier, bool focused) = 0;
	};
}