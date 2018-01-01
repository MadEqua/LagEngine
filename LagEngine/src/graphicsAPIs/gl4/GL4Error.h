#pragma once

#define GLEW_STATIC 
#include "GL/glew.h"

namespace Lag
{
	void printOpenGLErrors(const char* stmt, const char* fname, int line);
}

#ifdef _DEBUG
#define GL_ERROR_PRINT(stmt)\
			stmt;\
			Lag::printOpenGLErrors(#stmt, __FILE__, __LINE__);
#else
#define GL_ERROR_PRINT(stmt) stmt
#endif

#ifdef _DEBUG
#define GL_ERROR_PRINT_BLOCK(stmt) {\
			stmt;\
			Lag::printOpenGLErrors(#stmt, __FILE__, __LINE__);\
		}
#else
#define GL_ERROR_PRINT_BLOCK(stmt) stmt
#endif