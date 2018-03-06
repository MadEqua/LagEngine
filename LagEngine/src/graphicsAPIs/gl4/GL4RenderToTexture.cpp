#include "GL4RenderToTexture.h"
#include "GL4Texture.h"

#include "../../resources/Image.h"

using namespace Lag;

GL4RenderToTexture::GL4RenderToTexture(uint32 width, uint32 height, RenderPhase renderPhase) :
	RenderToTexture(width, height, renderPhase)
{
}

bool GL4RenderToTexture::loadImplementation()
{
	GL_ERROR_PRINT(glCreateFramebuffers(1, &handle))
	return true;
}

void GL4RenderToTexture::unloadImplementation()
{
	GL_ERROR_PRINT(glDeleteFramebuffers(1, &handle))
}

void GL4RenderToTexture::attachColorTexture(const ImageData &imageData, const TextureData &textureData,
	uint8 layer, uint8 attachment, uint8 mipmapLevel)
{
	Handle<Texture> gl4texture = createAndLoadTexture(generateTextureName("Color", layer, attachment), imageData, textureData);
	
	colorTextures[layer][attachment] = gl4texture;
	
	GL_ERROR_PRINT(glNamedFramebufferTexture(handle, 
		GL_COLOR_ATTACHMENT0 + attachment, static_cast<GL4Texture*>(gl4texture.get())->getHandle(), mipmapLevel))
}

void GL4RenderToTexture::attachDepthStencilTexture(const ImageData &imageData, const TextureData &textureData,
	uint8 layer, uint8 mipmapLevel)
{
	Handle<Texture> gl4texture = createAndLoadTexture(generateTextureName("DepthStencil", layer, 0), imageData, textureData);

	depthStencilTextures[layer] = gl4texture;

	GL_ERROR_PRINT(glNamedFramebufferTexture(handle,
		GL_DEPTH_STENCIL_ATTACHMENT, static_cast<GL4Texture*>(gl4texture.get())->getHandle(), mipmapLevel))
}

void GL4RenderToTexture::attachDepthTexture(const ImageData &imageData, const TextureData &textureData,
	uint8 layer, uint8 mipmapLevel)
{
	Handle<Texture> gl4texture = createAndLoadTexture(generateTextureName("Depth", layer, 0), imageData, textureData);

	depthTextures[layer] = gl4texture;
	
	GL_ERROR_PRINT(glNamedFramebufferTexture(handle,
		GL_DEPTH_ATTACHMENT, static_cast<GL4Texture*>(gl4texture.get())->getHandle(), mipmapLevel))
}

void GL4RenderToTexture::attachStencilTexture(const ImageData &imageData, const TextureData &textureData,
	uint8 layer, uint8 mipmapLevel)
{
	Handle<Texture> gl4texture = createAndLoadTexture(generateTextureName("Stencil", layer, 0), imageData, textureData);

	stencilTextures[layer] = gl4texture;
	
	GL_ERROR_PRINT(glNamedFramebufferTexture(handle,
		GL_STENCIL_ATTACHMENT, static_cast<GL4Texture*>(gl4texture.get())->getHandle(), mipmapLevel))
}

std::string GL4RenderToTexture::generateTextureName(const char* type, uint8 layer, uint8 attachment)
{
	return std::string(type) + std::to_string(handle) + std::to_string(layer) + std::to_string(attachment);
}

//TODO: better checks
bool GL4RenderToTexture::checkCompleteness() const
{
	if (depthTextures.empty() && depthStencilTextures.empty())
		return false;
	
	GLenum status = GL_ERROR_PRINT(glCheckNamedFramebufferStatus(handle, GL_DRAW_FRAMEBUFFER))
	if (status != GL_FRAMEBUFFER_COMPLETE)
		return false;
	return true;
}