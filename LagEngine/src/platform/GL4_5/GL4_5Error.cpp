#include "GL4_5Error.h"

#include "LogManager.h"
#include <sstream>

/*void Lag::printOpenGLErrors(const char* stmt, const char* fname, int line)
{
	GLenum ret = glGetError();
	if (ret != GL_NO_ERROR)
	{
		GLuint finite = 255;
		GLenum error = ret;

		while (error != GL_NO_ERROR && finite--)
			error = glGetError();

		if (error != GL_NO_ERROR)
			LogManager::getInstance().log(LogType::WARNING, LogVerbosity::VERBOSE, "OpenGL Error", "Failed to reset glGetError()");

		std::stringstream sstream;
		sstream << "Code: 0x" << std::hex << std::setw(4) << std::setfill('0') << ret 
			<< ", file: " << fname 
			<< ", line: " << std::dec << line
			<< ", statement: " << stmt;

		LogManager::getInstance().log(LogType::ERROR, LogVerbosity::VERBOSE, "OpenGL Error", sstream.str());
	}
}*/

void GLAPIENTRY Lag::openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                            const GLchar *message, const void *userParam) {
    std::string sourceString;
    switch (source) {
        case GL_DEBUG_SOURCE_API:
            sourceString = "API";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            sourceString = "SHADER_COMPILER";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            sourceString = "WINDOW_SYSTEM";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            sourceString = "THIRD_PARTY";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            sourceString = "APPLICATION";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            sourceString = "OTHER";
            break;
        default:
            sourceString = "Unknown";
    }

    LogType logType;
    std::string typeString;
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            typeString = "ERROR";
            logType = LogType::ERROR;
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            typeString = "DEPRECATED_BEHAVIOR";
            logType = LogType::WARNING;
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            typeString = "UNDEFINED_BEHAVIOR";
            logType = LogType::WARNING;
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            typeString = "PERFORMANCE";
            logType = LogType::WARNING;
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            typeString = "PORTABILITY";
            logType = LogType::WARNING;
            break;
        case GL_DEBUG_TYPE_MARKER:
            typeString = "MARKER";
            logType = LogType::INFO;
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            typeString = "PUSH_GROUP";
            logType = LogType::INFO;
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            typeString = "POP_GROUP";
            logType = LogType::INFO;
            break;
        case GL_DEBUG_TYPE_OTHER:
            typeString = "OTHER";
            logType = LogType::WARNING;
            break;
        default:
            typeString = "Unknown";
            logType = LogType::WARNING;
    }

    std::string severityString;
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            severityString = "HIGH";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            severityString = "MEDIUM";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            severityString = "LOW";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            severityString = "NOTIFICATION";
            break;
        default:
            severityString = "Unknown";
    }

    std::stringstream sstream;
    sstream << "Id: " << id << ". Source: " << sourceString << ", Type: " << typeString << ", Severity: "
            << severityString << ". " << message;
    LogManager::getInstance().log(logType, LogVerbosity::VERBOSE, "OpenGL Debug", sstream.str());
}