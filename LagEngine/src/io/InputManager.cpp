#include "InputManager.h"

#include "IKeyboardListener.h"
#include "ICursorListener.h"

using namespace Lag;

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

DEFINE_NOTIFY_METHOD(InputManager, onCursorMove, ICursorListener, ARGS(int x, int y), ARGS(x, y))
DEFINE_NOTIFY_METHOD(InputManager, onButtonPressed, ICursorListener, ARGS(int x, int y, int button, int modifiers), ARGS(x, y, button, modifiers))
DEFINE_NOTIFY_METHOD(InputManager, onButtonReleased, ICursorListener, ARGS(int x, int y, int button, int modifiers), ARGS(x, y, button, modifiers))

DEFINE_NOTIFY_METHOD(InputManager, onKeyPress, IKeyboardListener, ARGS(int key, int modifier), ARGS(key, modifier))
DEFINE_NOTIFY_METHOD(InputManager, onKeyRelease, IKeyboardListener, ARGS(int key, int modifier), ARGS(key, modifier))
DEFINE_NOTIFY_METHOD(InputManager, onKeyRepeat, IKeyboardListener, ARGS(int key, int modifier), ARGS(key, modifier))