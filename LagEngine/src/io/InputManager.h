#pragma once

#include "../core/ObserverPattern.h"

#include "IKeyboardListener.h"
#include "ICursorListener.h"

namespace Lag
{
	class ICursorListener;
	class IKeyboardListener;
	
	class InputManager
	{
	public:
		virtual ~InputManager() = default;

		//TODO: add poll methods
		//virtual bool isKeyPressed(Key key) = 0;

		LAG_GENERATE_OBSERVER_STORAGE(ICursorListener)
		LAG_GENERATE_OBSERVER_STORAGE(IKeyboardListener)

		LAG_GENERATE_NOTIFY_METHOD(onCursorMove, ICursorListener, LAG_ARGS(int x, int y), LAG_ARGS(x, y))
		LAG_GENERATE_NOTIFY_METHOD(onButtonPressed, ICursorListener, LAG_ARGS(int x, int y, int button, int modifiers), LAG_ARGS(x, y, button, modifiers))
		LAG_GENERATE_NOTIFY_METHOD(onButtonReleased, ICursorListener, LAG_ARGS(int x, int y, int button, int modifiers), LAG_ARGS(x, y, button, modifiers))

		LAG_GENERATE_NOTIFY_METHOD(onKeyPress, IKeyboardListener, LAG_ARGS(int key, int modifier), LAG_ARGS(key, modifier))
		LAG_GENERATE_NOTIFY_METHOD(onKeyRelease, IKeyboardListener, LAG_ARGS(int key, int modifier), LAG_ARGS(key, modifier))
		LAG_GENERATE_NOTIFY_METHOD(onKeyRepeat, IKeyboardListener, LAG_ARGS(int key, int modifier), LAG_ARGS(key, modifier))
	};
}