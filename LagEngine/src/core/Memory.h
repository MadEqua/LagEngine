#pragma once

//In debug mode we may want an helping hand finding memory leaks

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>

	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
	#define new new
#endif


void dumpMemoryLeaksReport();