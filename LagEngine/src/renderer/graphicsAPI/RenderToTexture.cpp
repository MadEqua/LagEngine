#include "RenderToTexture.h"

#include "Root.h"
#include "TextureManager.h"

using namespace Lag;

RenderToTexture::RenderToTexture(uint32 width, uint32 height, RenderPhase renderPhase) :
        RenderTarget(width, height, renderPhase) {
}

Handle<Texture> RenderToTexture::createAndLoadTexture(const std::string &name, const ImageData &imageData,
                                                      const TextureData &textureData) {
    TextureManager &texMan = Root::getInstance().getTextureManager();
    auto &builder = dynamic_cast<TextureBuilder &>(texMan.getBuilder());
    builder.setBuildCustom(imageData, textureData);
    return texMan.get(name);
}