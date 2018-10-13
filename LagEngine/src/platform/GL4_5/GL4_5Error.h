#pragma once

#include "GL/glew.h"

namespace Lag {
    //void printOpenGLErrors(const char* stmt, const char* fname, int line);

    void GLAPIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                           const GLchar *message, const void *userParam);
}

/*#ifdef ENABLE_DEBUG
#define GL_ERROR_PRINT(stmt)\
			stmt;\
			Lag::printOpenGLErrors(#stmt, __FILE__, __LINE__);
#else
#define GL_ERROR_PRINT(stmt) stmt;
#endif

#ifdef ENABLE_DEBUG
#define GL_ERROR_PRINT_BLOCK(stmt) {\
			stmt;\
			Lag::printOpenGLErrors(#stmt, __FILE__, __LINE__);\
		}
#else
#define GL_ERROR_PRINT_BLOCK(stmt) { stmt; }
#endif*/