#include "Memory.h"

void dumpMemoryLeaksReport() {
#ifdef ENABLE_DEBUG
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
#endif
}