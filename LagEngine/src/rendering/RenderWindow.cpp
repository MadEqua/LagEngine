#include "RenderWindow.h"

#include "IWindowListener.h"

using namespace Lag;

RenderWindow::RenderWindow(const RenderWindowParameters &parameters) : parameters(parameters)
{
}

RenderWindow::~RenderWindow()
{
}

DEFINE_NOTIFY_METHOD(RenderWindow, onMove, IWindowListener, ARGS(int x, int y), ARGS(x, y))
DEFINE_NOTIFY_METHOD(RenderWindow, onResize, IWindowListener, ARGS(int width, int height), ARGS(width, height))
DEFINE_NOTIFY_METHOD(RenderWindow, onClose, IWindowListener, ARGS(), ARGS())
DEFINE_NOTIFY_METHOD(RenderWindow, onFocusChange, IWindowListener, ARGS(bool focused), ARGS(focused))