#pragma once

#include "IRenderTarget.h"
#include "RenderWindowParameters.h"
#include "../core/ObserverPattern.h"

namespace Lag
{
	class IWindowListener;
	
	class RenderWindow : public IRenderTarget
	{
	public:
		explicit RenderWindow(const RenderWindowParameters &parameters);
		virtual ~RenderWindow();

		//Process messages coming from the OS
		virtual void processEvents() = 0;

		//Observer Pattern stuff
		GENERATE_OBSERVER_STORAGE(IWindowListener)
		DECLARE_NOTIFY_METHOD(onMove, ARGS(int x, int y))
		DECLARE_NOTIFY_METHOD(onResize, ARGS(int width, int height))
		DECLARE_NOTIFY_METHOD(onClose, ARGS())
		DECLARE_NOTIFY_METHOD(onFocusChange, ARGS(bool focused))

	protected:
		RenderWindowParameters parameters;
	};
}