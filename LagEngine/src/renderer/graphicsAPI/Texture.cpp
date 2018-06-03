#include "Texture.h"

#include <algorithm>

#include "ImageManager.h"
#include "Image.h"
#include "Root.h"
#include "LogManager.h"

using namespace Lag;

Texture::Texture(const std::string &imageName, const TextureData &data) :
        textureData(data) {
    imageNames.push_back(imageName);
}

Texture::Texture(const std::vector<std::string> &imageNames, const TextureData &data) :
        imageNames(imageNames),
        textureData(data) {
}

Texture::Texture(const ImageData &imageData, const TextureData &textureData) :
        imageData(imageData),
        textureData(textureData) {
}

bool Texture::loadImplementation() {
    if (!imageNames.empty()) {
        int i = 0;
        ImageManager &imageManager = Root::getInstance().getImageManager();
        for (auto &name : imageNames) {
            Handle<Image> image = imageManager.get(name);

            if (image.isValid()) {
                if (i == 0)
                    imageData = image->getImageData();

                images.push_back(image);
            }
            else {
                LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                              "Texture", "Cannot get Image: " + name);
                return false;
            }
            i++;
        }
    }
    return true;
}

void Texture::unloadImplementation() {
    images.clear();
}

bool Texture::containsImageWithName(const std::string &imageName) const {
    return std::find(imageNames.begin(), imageNames.end(), imageName) != imageNames.end();
}