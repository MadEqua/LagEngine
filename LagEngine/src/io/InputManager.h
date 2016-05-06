#pragma once

#include "../core/ObserverPattern.h"

namespace Lag
{
	class ICursorListener;
	class IKeyboardListener;
	
	class InputManager
	{
	public:
		InputManager();
		virtual ~InputManager();

		//TODO: add poll methods
		//virtual bool isKeyPressed(Key key) = 0;

		GENERATE_OBSERVER_STORAGE(ICursorListener)
		GENERATE_OBSERVER_STORAGE(IKeyboardListener)

		DECLARE_NOTIFY_METHOD(onCursorMove, ARGS(int x, int y))
		DECLARE_NOTIFY_METHOD(onButtonPressed, ARGS(int x, int y, int button, int modifiers))
		DECLARE_NOTIFY_METHOD(onButtonReleased, ARGS(int x, int y, int button, int modifiers))

		DECLARE_NOTIFY_METHOD(onKeyPress, ARGS(int key, int modifier))
		DECLARE_NOTIFY_METHOD(onKeyRelease, ARGS(int key, int modifier))
		DECLARE_NOTIFY_METHOD(onKeyRepeat, ARGS(int key, int modifier))
	};
}