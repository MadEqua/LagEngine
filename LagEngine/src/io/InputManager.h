#pragma once

#include "../core/ObserverPattern.h"

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

		LAG_DECLARE_NOTIFY_METHOD(onCursorMove, LAG_ARGS(int x, int y))
		LAG_DECLARE_NOTIFY_METHOD(onButtonPressed, LAG_ARGS(int x, int y, int button, int modifiers))
		LAG_DECLARE_NOTIFY_METHOD(onButtonReleased, LAG_ARGS(int x, int y, int button, int modifiers))

		LAG_DECLARE_NOTIFY_METHOD(onKeyPress, LAG_ARGS(int key, int modifier))
		LAG_DECLARE_NOTIFY_METHOD(onKeyRelease, LAG_ARGS(int key, int modifier))
		LAG_DECLARE_NOTIFY_METHOD(onKeyRepeat, LAG_ARGS(int key, int modifier))
	};
}