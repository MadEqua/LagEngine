#include "GL4RenderToTexture.h"
#include "GL4Texture.h"

using namespace Lag;

GL4RenderToTexture::GL4RenderToTexture(uint32 width, uint32 height) :
	RenderToTexture(width, height)
{
	GL_ERROR_CHECK(glCreateFramebuffers(1, &handle))
}

GL4RenderToTexture::~GL4RenderToTexture()
{
	GL_ERROR_CHECK(glDeleteFramebuffers(1, &handle))
}

bool GL4RenderToTexture::initialize()
{
	return true;
}

void GL4RenderToTexture::destroy()
{
}

void GL4RenderToTexture::attachColorTexture(const Texture &texture, uint8 attachment, uint8 mipmapLevel)
{
	colorTextures[attachment] = &texture;
	
	const GL4Texture &gl4texture = static_cast<const GL4Texture&>(texture);
	GL_ERROR_CHECK(glNamedFramebufferTexture(handle, 
		GL_COLOR_ATTACHMENT0 + attachment, gl4texture.getHandle(), mipmapLevel))
}

void GL4RenderToTexture::attachDepthStencilTexture(const Texture &texture, uint8 mipmapLevel)
{
	depthStencilTextures.push_back(&texture);

	const GL4Texture &gl4texture = static_cast<const GL4Texture&>(texture);
	GL_ERROR_CHECK(glNamedFramebufferTexture(handle,
		GL_DEPTH_STENCIL_ATTACHMENT, gl4texture.getHandle(), mipmapLevel))
}

void GL4RenderToTexture::attachDepthTexture(const Texture &texture, uint8 mipmapLevel)
{
	depthTextures.push_back(&texture);
	
	const GL4Texture &gl4texture = static_cast<const GL4Texture&>(texture);
	GL_ERROR_CHECK(glNamedFramebufferTexture(handle,
		GL_DEPTH_ATTACHMENT, gl4texture.getHandle(), mipmapLevel))
}

void GL4RenderToTexture::attachStencilTexture(const Texture &texture, uint8 mipmapLevel)
{
	stencilTextures.push_back(&texture);
	
	const GL4Texture &gl4texture = static_cast<const GL4Texture&>(texture);
	GL_ERROR_CHECK(glNamedFramebufferTexture(handle,
		GL_STENCIL_ATTACHMENT, gl4texture.getHandle(), mipmapLevel))
}

//TODO: better checks
bool GL4RenderToTexture::checkFboCompleteness() const
{
	if (depthTextures.empty() && depthStencilTextures.empty())
		return false;
	
	GLenum status = GL_ERROR_CHECK(glCheckNamedFramebufferStatus(handle, GL_DRAW_FRAMEBUFFER))
	if (status != GL_FRAMEBUFFER_COMPLETE)
		return false;

	return true;
}