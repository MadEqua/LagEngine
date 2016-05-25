#include "GL4GraphicsApi.h"

#include "../../io/log/LogManager.h"
#include "../../renderer/Renderer.h"
#include "../../renderer/IndexData.h"

#include "GL4Error.h"

using namespace Lag;

GL4GraphicsAPI::GL4GraphicsAPI()
{
	//TODO: better initialization stuff
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::string errorString = reinterpret_cast<const char*>(glewGetErrorString(err));
		LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_ERROR,
			"GL4GraphicsAPI", "Failed to initialize GLEW: " + errorString);
	}

	if (GLEW_VERSION_4_5)
	{
		LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
			"GL4GraphicsAPI", "OpenGL 4.5 is supported!");
	}
}

GL4GraphicsAPI::~GL4GraphicsAPI()
{
}

void GL4GraphicsAPI::renderVertices(RenderMode mode, uint32 first, uint32 count)
{
	GL_ERROR_CHECK(glDrawArrays(convertRenderModeToGLenum(mode), first, count))
}

void GL4GraphicsAPI::renderIndexed(RenderMode mode, uint32 first, IndexType indexType, uint32 count, uint32 baseVertex)
{
	uint32 indexByteSize;
	GLenum type;
	switch (indexType)
	{
	case LAG_IDX_TYPE_UINT8:
		type = GL_UNSIGNED_BYTE;
		indexByteSize = 1;
		break;
	case LAG_IDX_TYPE_UINT16:
		type = GL_UNSIGNED_SHORT;
		indexByteSize = 2;
		break;
	case LAG_IDX_TYPE_UINT32:
		type = GL_UNSIGNED_INT;
		indexByteSize = 4;
		break;
	default:
		indexByteSize = 4;
		type = GL_UNSIGNED_INT;
		break;
	}
	
	uint32 offset = first * indexByteSize;
	GL_ERROR_CHECK(glDrawElementsBaseVertex(convertRenderModeToGLenum(mode), count, type, reinterpret_cast<void*>(offset), baseVertex))
}

void GL4GraphicsAPI::clearColorBuffer(float value[4])
{
	GL_ERROR_CHECK(glClearBufferfv(GL_COLOR, 0, value))
}

void GL4GraphicsAPI::clearDepthBuffer(float value)
{
	GL_ERROR_CHECK(glClearBufferfv(GL_DEPTH, 0, &value))
}

void GL4GraphicsAPI::clearStencilBuffer(int32 value)
{
	GL_ERROR_CHECK(glClearBufferiv(GL_STENCIL, 0, &value))
}

void GL4GraphicsAPI::clearDepthAndStencilBuffer(float depth, int32 stencil)
{
	GL_ERROR_CHECK(glClearBufferfi(GL_DEPTH_STENCIL, 0, depth, stencil))
}

GLenum GL4GraphicsAPI::convertRenderModeToGLenum(RenderMode renderMode)
{
	switch (renderMode)
	{
	case LAG_RENDER_MODE_TRIANGLES:
		return GL_TRIANGLES;
	case LAG_RENDER_MODE_TRIANGLE_STRIP:
		return GL_TRIANGLE_STRIP;
	case LAG_RENDER_MODE_TRIANGLE_FAN:
		return GL_TRIANGLE_FAN;
	case LAG_RENDER_MODE_LINES:
		return GL_LINES;
	case LAG_RENDER_MODE_LINE_STRIP:
		return GL_LINE_STRIP;
	case LAG_RENDER_MODE_LINE_LOOP:
		return GL_LINE_LOOP;
	case LAG_RENDER_MODE_POINTS:
		return GL_POINTS;
	case LAG_RENDER_MODE_PATCHES:
		return GL_PATCHES;
	default:
		return GL_POINTS;
	}
}