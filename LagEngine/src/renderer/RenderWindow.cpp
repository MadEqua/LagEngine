#include "RenderWindow.h"

#include "../InitializationParameters.h"
#include "IWindowListener.h"
#include "../Root.h"

using namespace Lag;

RenderWindow::RenderWindow(const InitializationParameters &parameters) :
	RenderTarget(parameters.width, parameters.height, true),
	initializationParameters(parameters)
{
}

RenderWindow::~RenderWindow()
{
}

LAG_DEFINE_NOTIFY_METHOD(RenderWindow, onMove, IWindowListener, LAG_ARGS(int x, int y), LAG_ARGS(x, y))
LAG_DEFINE_NOTIFY_METHOD(RenderWindow, onClose, IWindowListener, LAG_ARGS(), LAG_ARGS())
LAG_DEFINE_NOTIFY_METHOD(RenderWindow, onFocusChange, IWindowListener, LAG_ARGS(bool focused), LAG_ARGS(focused))