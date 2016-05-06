#include "Root.h"

int main()
{
	Lag::Root::getInstance().initializeLag("startup.ini");
	Lag::Root::getInstance().startRenderingLoop();
}