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

LAG_DEFINE_NOTIFY_METHOD(InputManager, onCursorMove, ICursorListener, LAG_ARGS(int x, int y), LAG_ARGS(x, y))
LAG_DEFINE_NOTIFY_METHOD(InputManager, onButtonPressed, ICursorListener, LAG_ARGS(int x, int y, int button, int modifiers), LAG_ARGS(x, y, button, modifiers))
LAG_DEFINE_NOTIFY_METHOD(InputManager, onButtonReleased, ICursorListener, LAG_ARGS(int x, int y, int button, int modifiers), LAG_ARGS(x, y, button, modifiers))

LAG_DEFINE_NOTIFY_METHOD(InputManager, onKeyPress, IKeyboardListener, LAG_ARGS(int key, int modifier), LAG_ARGS(key, modifier))
LAG_DEFINE_NOTIFY_METHOD(InputManager, onKeyRelease, IKeyboardListener, LAG_ARGS(int key, int modifier), LAG_ARGS(key, modifier))
LAG_DEFINE_NOTIFY_METHOD(InputManager, onKeyRepeat, IKeyboardListener, LAG_ARGS(int key, int modifier), LAG_ARGS(key, modifier))