#include "RenderWindow.h"

#include "InitializationParameters.h"

using namespace Lag;

RenderWindow::RenderWindow(const InitializationParameters &parameters) :
        RenderTarget(parameters.width, parameters.height, RenderPhase::COLOR, true),
        initializationParameters(parameters) {
}