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

void GL4RenderToTexture::attachColorTexture(const ImageData &imageData, const TextureData &textureData,
	uint8 layer, uint8 attachment, uint8 mipmapLevel)
{
	const GL4Texture *gl4texture = 
		static_cast<const GL4Texture*>(createAndLoadTexture(generateTextureName("Color", layer, attachment),
			imageData, textureData));
	
	colorTextures[layer][attachment] = gl4texture;
	
	GL_ERROR_CHECK(glNamedFramebufferTexture(handle, 
		GL_COLOR_ATTACHMENT0 + attachment, gl4texture->getHandle(), mipmapLevel))
}

void GL4RenderToTexture::attachDepthStencilTexture(const ImageData &imageData, const TextureData &textureData,
	uint8 layer, uint8 mipmapLevel)
{
	const GL4Texture *gl4texture =
		static_cast<const GL4Texture*>(createAndLoadTexture(generateTextureName("DepthStencil", layer, 0), 
			imageData, textureData));

	depthStencilTextures[layer] = gl4texture;

	GL_ERROR_CHECK(glNamedFramebufferTexture(handle,
		GL_DEPTH_STENCIL_ATTACHMENT, gl4texture->getHandle(), mipmapLevel))
}

void GL4RenderToTexture::attachDepthTexture(const ImageData &imageData, const TextureData &textureData,
	uint8 layer, uint8 mipmapLevel)
{
	const GL4Texture *gl4texture =
		static_cast<const GL4Texture*>(createAndLoadTexture(generateTextureName("Depth", layer, 0),
			imageData, textureData));

	depthTextures[layer] = gl4texture;
	
	GL_ERROR_CHECK(glNamedFramebufferTexture(handle,
		GL_DEPTH_ATTACHMENT, gl4texture->getHandle(), mipmapLevel))
}

void GL4RenderToTexture::attachStencilTexture(const ImageData &imageData, const TextureData &textureData,
	uint8 layer, uint8 mipmapLevel)
{
	const GL4Texture *gl4texture =
		static_cast<const GL4Texture*>(createAndLoadTexture(generateTextureName("Stencil", layer, 0),
			imageData, textureData));

	stencilTextures[layer] = gl4texture;
	
	GL_ERROR_CHECK(glNamedFramebufferTexture(handle,
		GL_STENCIL_ATTACHMENT, gl4texture->getHandle(), mipmapLevel))
}

std::string GL4RenderToTexture::generateTextureName(const char* type, uint8 layer, uint8 attachment)
{
	return std::string(type) + std::to_string(handle) +
		std::to_string(layer) + std::to_string(attachment);
}

//TODO: better checks
bool GL4RenderToTexture::checkCompleteness() const
{
	if (depthTextures.empty() && depthStencilTextures.empty())
		return false;
	
	GLenum status = GL_ERROR_CHECK(glCheckNamedFramebufferStatus(handle, GL_DRAW_FRAMEBUFFER))
	if (status != GL_FRAMEBUFFER_COMPLETE)
		return false;

	return true;
}