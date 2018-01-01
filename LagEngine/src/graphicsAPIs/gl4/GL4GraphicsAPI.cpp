#include "GL4GraphicsApi.h"

#include "../../io/log/LogManager.h"
#include "../../renderer/Renderer.h"
#include "../../renderer/IndexData.h"
#include "../../renderer/Color.h"

#include "GL4InputDescription.h"
#include "GL4GpuProgram.h"
#include "GL4GpuBuffer.h"
#include "GL4Texture.h"
#include "GL4RenderToTexture.h"

#include "GL4Error.h"

using namespace Lag;

GL4GraphicsAPI::GL4GraphicsAPI()
{
	//TODO: better initialization stuff
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::string errorString = reinterpret_cast<const char*>(glewGetErrorString(err));
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			"GL4GraphicsAPI", "Failed to initialize GLEW: " + errorString);
		return;
	}

	if (GLEW_VERSION_4_5)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
			"GL4GraphicsAPI", "OpenGL 4.5 is supported!");
	}

	GLint res;
	GL_ERROR_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0))
	GL_ERROR_CHECK(glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, 
		GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING, &res))

	if (res == GL_LINEAR)
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
			"GL4GraphicsAPI", "Linear RGB Default Framebuffer.");
	else if (res == GL_SRGB) 
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
			"GL4GraphicsAPI", "sRGB Default Framebuffer.");
		
		//enable auto Linear RGB to sRGB conversion when writing to sRGB framebuffers
		GL_ERROR_CHECK(glEnable(GL_FRAMEBUFFER_SRGB)) 
	}

	GL_ERROR_CHECK(glGetIntegerv(GL_SAMPLES, &res))
	if (res > 0) 
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
			"GL4GraphicsAPI", "Multisampled Default Framebuffer. Samples: " + res);
		GL_ERROR_CHECK(glEnable(GL_MULTISAMPLE))
	}
	else 
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
			"GL4GraphicsAPI", "Non-Multisampled Default Framebuffer.");
	}

	GL_ERROR_CHECK(glEnable(GL_DEPTH_TEST))
	GL_ERROR_CHECK(glDepthFunc(GL_LESS))

	GL_ERROR_CHECK(glEnable(GL_CULL_FACE))
	GL_ERROR_CHECK(glCullFace(GL_BACK))
}

RenderToTexture* GL4GraphicsAPI::createRenderToTexture(uint32 width, uint32 height)
{
	return new GL4RenderToTexture(width, height);
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

void GL4GraphicsAPI::clearColorBuffer(const Color &color)
{
	GL_ERROR_CHECK(glClearBufferfv(GL_COLOR, 0, color.getRGBAfloat()))
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

void GL4GraphicsAPI::bindVertexBuffer(const GpuBuffer &vertexBuffer)
{
	GLuint handle = static_cast<const GL4GpuBuffer&>(vertexBuffer).getHandle();
	GL_ERROR_CHECK(glBindBuffer(GL_ARRAY_BUFFER, handle))
}

void GL4GraphicsAPI::bindIndexBuffer(const GpuBuffer &indexBuffer)
{
	GLuint handle = static_cast<const GL4GpuBuffer&>(indexBuffer).getHandle();
	GL_ERROR_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle))
}

void GL4GraphicsAPI::bindGpuProgram(const GpuProgram &gpuProgram)
{
	GLuint handle = static_cast<const GL4GpuProgram&>(gpuProgram).getHandle();
	GL_ERROR_CHECK(glUseProgram(handle))
}

void GL4GraphicsAPI::bindInputDescription(const InputDescription &inputDescription)
{
	GLuint handle = static_cast<const GL4InputDescription&>(inputDescription).getHandle();
	GL_ERROR_CHECK(glBindVertexArray(handle))
}

void GL4GraphicsAPI::bindRenderTarget(const RenderTarget &renderTarget)
{
	GLuint handle = 0;
	if(!renderTarget.isMainWindow())
		handle = static_cast<const GL4RenderToTexture&>(renderTarget).getHandle();
	GL_ERROR_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, handle))
}

void GL4GraphicsAPI::bindViewport(uint32 x, uint32 y, uint32 width, uint32 height)
{
	GL_ERROR_CHECK(glViewport(x, y, width, height))
}

void GL4GraphicsAPI::bindTexture(const Texture &texture, uint8 unit)
{
	const GL4Texture& GL4Tex = static_cast<const GL4Texture&>(texture);
	GL_ERROR_CHECK(glBindTextureUnit(unit, GL4Tex.getHandle()))
}

void GL4GraphicsAPI::setDepthTestEnabled(bool enabled)
{
	if (enabled)
		GL_ERROR_CHECK_BLOCK(glEnable(GL_DEPTH_TEST))
	else
		GL_ERROR_CHECK_BLOCK(glDisable(GL_DEPTH_TEST))
}

void GL4GraphicsAPI::setDepthWritingEnabled(bool enabled)
{
	GL_ERROR_CHECK(glDepthMask(enabled ? GL_TRUE : GL_FALSE))
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