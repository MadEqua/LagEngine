#include "Root.h"

int main()
{
	Lag::RenderWindowParameters params;
	
	Lag::Root::getInstance().initializeLag(params);
	Lag::Root::getInstance().startRenderingLoop();
}