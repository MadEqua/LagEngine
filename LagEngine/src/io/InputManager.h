#pragma once

#include "../core/ObserverPattern.h"
#include "IKeyboardListener.h"
#include "ICursorListener.h"
#include "Keys.h"

namespace Lag
{
	class InputManager
	{
	public:
		InputManager() {}
		virtual ~InputManager() {}

		//TODO: add poll methods
		//virtual bool isKeyPressed(Key key) = 0;

		GENERATE_OBSERVER_STORAGE(ICursorListener)
		GENERATE_NOTIFY_METHOD(ICursorListener, onCursorMove, ARGS(int x, int y), ARGS(x, y))
		GENERATE_NOTIFY_METHOD(ICursorListener, onButtonPressed, ARGS(int x, int y, int button, int modifiers), ARGS(x, y, button, modifiers))
		GENERATE_NOTIFY_METHOD(ICursorListener, onButtonReleased, ARGS(int x, int y, int button, int modifiers), ARGS(x, y, button, modifiers))

		GENERATE_OBSERVER_STORAGE(IKeyboardListener)
		GENERATE_NOTIFY_METHOD(IKeyboardListener, onKeyPress, ARGS(int key, int modifier), ARGS(key, modifier))
		GENERATE_NOTIFY_METHOD(IKeyboardListener, onKeyRelease, ARGS(int key, int modifier), ARGS(key, modifier))
		GENERATE_NOTIFY_METHOD(IKeyboardListener, onKeyRepeat, ARGS(int key, int modifier), ARGS(key, modifier))
	};
}