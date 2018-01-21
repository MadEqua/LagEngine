#include "RenderToTexture.h"

#include "../../Root.h"
#include "../../resources/TextureManager.h"

using namespace Lag;

RenderToTexture::RenderToTexture(uint32 width, uint32 height) :
	RenderTarget(width, height)
{
}

const Texture* RenderToTexture::createAndLoadTexture(const std::string &name, const ImageData &imageData, const TextureData &textureData)
{
	TextureManager &texMan = Root::getInstance().getTextureManager();
	TextureBuilder &builder = static_cast<TextureBuilder&>(texMan.getBuilder());
	builder.setBuildCustom(imageData, textureData);
	return texMan.get(name);
}

void RenderToTexture::freeTextures()
{
	TextureManager &texMan = Root::getInstance().getTextureManager();
	for (auto &colorTexList : colorTextures)
		for (auto &colorTex : colorTexList.second) 
			texMan.returnObject(const_cast<Texture*>(colorTex.second));
	colorTextures.clear();

	for (auto &depthTex : depthTextures)
		texMan.returnObject(const_cast<Texture*>(depthTex.second));
	depthTextures.clear();

	for (auto &stencilTex : stencilTextures)
		texMan.returnObject(const_cast<Texture*>(stencilTex.second));
	stencilTextures.clear();

	for (auto &depthStencilTex : depthStencilTextures)
		texMan.returnObject(const_cast<Texture*>(depthStencilTex.second));
	depthStencilTextures.clear();
}