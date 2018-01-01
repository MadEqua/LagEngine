#pragma once

#define GLEW_STATIC 
#include "GL/glew.h"

namespace Lag
{
	void CheckOpenGLError(const char* stmt, const char* fname, int line);
}

#ifdef _DEBUG
#define GL_ERROR_CHECK(stmt)\
			stmt;\
			Lag::CheckOpenGLError(#stmt, __FILE__, __LINE__);
#else
#define GL_CHECK(stmt) stmt
#endif

#ifdef _DEBUG
#define GL_ERROR_CHECK_BLOCK(stmt) {\
			stmt;\
			Lag::CheckOpenGLError(#stmt, __FILE__, __LINE__);\
		}
#else
#define GL_CHECK(stmt) stmt
#endif