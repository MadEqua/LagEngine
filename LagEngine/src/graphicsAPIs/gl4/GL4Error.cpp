#include "GL4Error.h"

#include "../../io/log/LogManager.h"
#include <sstream>

void Lag::printOpenGLErrors(const char* stmt, const char* fname, int line)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::stringstream sstream;
		sstream << "Code: " << err << ", file: " << fname << ", line: " << line << ", statement: " << stmt;

		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_VERBOSE,
			"OpenGL Error", sstream.str());
	}
}