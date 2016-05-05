#pragma once

#include "IRenderTarget.h"
#include "RenderWindowParameters.h"
#include "../core/ObserverPattern.h"
#include "IWindowListener.h"

namespace Lag
{
	class RenderWindow : public IRenderTarget
	{
	public:
		RenderWindow(const RenderWindowParameters &parameters) : parameters(parameters) {}
		virtual ~RenderWindow() {}

		//Process messages coming from the OS
		virtual void processEvents() = 0;

		//Observer Pattern stuff
		GENERATE_OBSERVER_STORAGE(IWindowListener)
		GENERATE_NOTIFY_METHOD(IWindowListener, onMove, ARGS(int x, int y), ARGS(x, y))
		GENERATE_NOTIFY_METHOD(IWindowListener, onResize, ARGS(int width, int height), ARGS(width, height))
		GENERATE_NOTIFY_METHOD(IWindowListener, onClose, ARGS(), ARGS())
		GENERATE_NOTIFY_METHOD(IWindowListener, onFocusChange, ARGS(bool focused), ARGS(focused))

	protected:
		RenderWindowParameters parameters;
	};
}