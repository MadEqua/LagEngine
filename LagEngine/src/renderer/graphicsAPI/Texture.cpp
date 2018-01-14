#include "Texture.h"

#include "../../resources/ImageManager.h"
#include "../../resources/Image.h"
#include "../../Root.h"
#include "../../io/log/LogManager.h"

using namespace Lag;

Texture::Texture(const std::string &imageName, const TextureData &data) :
	textureData(data)
{
	imageNames.push_back(imageName);
}

Texture::Texture(const std::vector<std::string> &imageNames, const TextureData &data) :
	imageNames(imageNames),
	textureData(data)
{
}

Texture::Texture(const ImageData &imageData, const TextureData &textureData) :
	imageData(imageData),
	textureData(textureData)
{
}

bool Texture::loadImplementation()
{
	if (!imageNames.empty())
	{
		int i = 0;
		ImageManager &imageManager = Root::getInstance().getImageManager();
		for (auto name : imageNames)
		{	
			Image *image = imageManager.get(name);
			if (image != nullptr)
			{
				if (!imageManager.load(name))
					return false;
				
				if (i == 0)
					imageData = image->getData();

				images.push_back(image);
			}
			else
			{
				LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
					"Texture", "Trying to use a non-declared Image: " + name);
				return false;
			}
			i++;
		}
	}
	return true;
}

void Texture::unloadImplementation()
{
	images.clear();
	imageNames.clear();
}