#include "RenderWindow.h"

#include "../InitializationParameters.h"

using namespace Lag;

RenderWindow::RenderWindow(const InitializationParameters &parameters) :
	RenderTarget(parameters.width, parameters.height, LAG_RENDER_PHASE_COLOR, true),
	initializationParameters(parameters)
{
}