#include "Memory.h"

void dumpMemoryLeaksReport() {
#ifdef ENABLE_DEBUG_MACRO
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
#endif
}