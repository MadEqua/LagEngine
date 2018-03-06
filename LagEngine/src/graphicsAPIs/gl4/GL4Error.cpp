#include "GL4Error.h"

#include "../../io/log/LogManager.h"
#include <iomanip>
#include <sstream>

/*void Lag::printOpenGLErrors(const char* stmt, const char* fname, int line)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::stringstream sstream;
		sstream << "Code: " << err << ", file: " << fname << ", line: " << line << ", statement: " << stmt;

		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_VERBOSE,
			"OpenGL Error", sstream.str());
	}
}*/

void Lag::printOpenGLErrors(const char* stmt, const char* fname, int line)
{
	GLenum ret = glGetError();
	if (ret != GL_NO_ERROR)
	{
		GLuint finite = 255;
		GLenum error = ret;

		while (error != GL_NO_ERROR && finite--)
			error = glGetError();

		if (error != GL_NO_ERROR)
			LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_VERBOSE, "OpenGL Error", "Failed to reset glGetError()");

		std::stringstream sstream;
		sstream << "Code: 0x" << std::hex << std::setw(4) << std::setfill('0') << ret 
			<< ", file: " << fname 
			<< ", line: " << std::dec << line
			<< ", statement: " << stmt;

		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_VERBOSE, "OpenGL Error", sstream.str());
	}
}