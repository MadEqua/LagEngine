#include "RenderWindow.h"

#include "../InitializationParameters.h"
#include "IWindowListener.h"
#include "../Root.h"

using namespace Lag;

RenderWindow::RenderWindow(const InitializationParameters &parameters) :
	RenderTarget(parameters.width, parameters.height, LAG_RENDER_PHASE_COLOR, true),
	initializationParameters(parameters)
{
}

RenderWindow::~RenderWindow()
{
}

LAG_DEFINE_NOTIFY_METHOD(RenderWindow, onMove, IWindowListener, LAG_ARGS(RenderWindow &notifier, int x, int y), LAG_ARGS(notifier, x, y))
LAG_DEFINE_NOTIFY_METHOD(RenderWindow, onClose, IWindowListener, LAG_ARGS(RenderWindow &notifier), LAG_ARGS(notifier))
LAG_DEFINE_NOTIFY_METHOD(RenderWindow, onFocusChange, IWindowListener, LAG_ARGS(RenderWindow &notifier, bool focused), LAG_ARGS(notifier, focused))