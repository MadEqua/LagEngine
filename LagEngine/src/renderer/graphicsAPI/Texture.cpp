#include "Texture.h"

#include "../../resources/ImageManager.h"
#include "../../resources/Image.h"
#include "../../Root.h"
#include "../../io/log/LogManager.h"

using namespace Lag;

Texture::Texture(const std::string &imageName, const TextureData &data) :
	data(data)
{
	imageNames.push_back(imageName);
}

Texture::Texture(const std::vector<std::string> &imageNames, const TextureData &data) :
	imageNames(imageNames),
	data(data)
{
}

Texture::~Texture()
{
}

bool Texture::loadImplementation()
{
	ImageManager &imageManager = Root::getInstance().getImageManager();
	for (auto name : imageNames)
	{
		Image *image = static_cast<Image*>(imageManager.get(name));
		
		if (image != nullptr)
			images.push_back(image);
		else
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
				"Texture", "Trying to use load using a non-declared Image: " + name);
			return false;
		}
	}
	return true;
}

void Texture::unloadImplementation()
{
}