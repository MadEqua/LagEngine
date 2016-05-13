#pragma once

#include "RenderTarget.h"
#include "../InitializationParameters.h"
#include "../core/ObserverPattern.h"

namespace Lag
{
	class IWindowListener;
	
	class RenderWindow : public RenderTarget
	{
		//Observer Pattern stuff
		GENERATE_OBSERVER_STORAGE(IWindowListener)
		DECLARE_NOTIFY_METHOD(onMove, ARGS(int x, int y))
		DECLARE_NOTIFY_METHOD(onResize, ARGS(int width, int height))
		DECLARE_NOTIFY_METHOD(onClose, ARGS())
		DECLARE_NOTIFY_METHOD(onFocusChange, ARGS(bool focused))

	public:
		explicit RenderWindow(const InitializationParameters &parameters);
		virtual ~RenderWindow();

		//Process messages coming from the OS
		virtual void processEvents() = 0;
	protected:
		InitializationParameters parameters;
	};
}