#include "RenderToTexture.h"
#include "../../resources/Image.h"
#include "../../renderer/graphicsAPI/Texture.h"
#include "../../Root.h"
#include "../../resources/TextureManager.h"

using namespace Lag;

RenderToTexture::RenderToTexture(uint32 width, uint32 height) :
	RenderTarget(width, height)
{
}

bool RenderToTexture::initialize()
{
	return checkCompleteness();
}

void RenderToTexture::destroy()
{
}

const Texture* RenderToTexture::createAndLoadTexture(const std::string &name, const ImageData &imageData, const TextureData &textureData)
{
	TextureManager &texMan = Root::getInstance().getTextureManager();
	texMan.create(name, imageData, textureData);
	Texture *tex = texMan.get(name);
	tex->load();
	return tex;
}