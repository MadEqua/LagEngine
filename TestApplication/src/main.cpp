#include "TestApplication.h"

int main()
{
	TestApplication app;

	if (!app.start())
		return 1;

	return 0;
}