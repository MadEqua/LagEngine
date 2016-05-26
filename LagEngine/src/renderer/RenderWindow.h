#pragma once

#include "RenderTarget.h"
#include "../core/ObserverPattern.h"

namespace Lag
{
	class IWindowListener;
	class InitializationParameters;
	
	class RenderWindow : public RenderTarget
	{
		//Observer Pattern stuff
		LAG_GENERATE_OBSERVER_STORAGE(IWindowListener)
		LAG_DECLARE_NOTIFY_METHOD(onMove, LAG_ARGS(int x, int y))
		LAG_DECLARE_NOTIFY_METHOD(onClose, LAG_ARGS())
		LAG_DECLARE_NOTIFY_METHOD(onFocusChange, LAG_ARGS(bool focused))

	public:
		explicit RenderWindow(const InitializationParameters &parameters);
		virtual ~RenderWindow();

		//Process messages coming from the OS
		virtual void processEvents() = 0;

	protected:
		const InitializationParameters &initializationParameters;
	};
}