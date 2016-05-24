#include "GL4Error.h"

#include "../../io/log/LogManager.h"
#include <sstream>

void Lag::CheckOpenGLError(const char* stmt, const char* fname, int line)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::stringstream sstream;
		sstream << "Code: " << err << ", file: " << fname << ", line: " << line << ", statement: " << stmt;

		LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_VERBOSE, LAG_LOG_TYPE_ERROR,
			"OpenGL Error", sstream.str());
	}
}