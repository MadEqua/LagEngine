#include "TestApplication.h"

//Visual Leak Detector
//#include "vld.h"

int main()
{
	TestApplication app;

	if (!app.start())
		return 1;

	return 0;
}