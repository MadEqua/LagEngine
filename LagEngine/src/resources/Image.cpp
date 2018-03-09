#include "Image.h"

#include "../io/log/LogManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stbi/stb_image.h"

using namespace Lag;

Image::Image(const std::string &path, const ImageData &imageData) :
	XmlResource(path),
	imageData(imageData),
	dataPtr(nullptr)
{
}

bool Image::loadImplementation()
{
	int w, h;
	int componentsPerPixel; //This can be compared to the XML declared value and check for inconsistencies
	stbi_set_flip_vertically_on_load(true); //stbi first pixel is top left. OpenGL expects bottom left.
	stbi_uc *pixels = stbi_load(path.c_str(), &w, &h, &componentsPerPixel, 0);

	
	if (pixels == NULL)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			"Image", "Could not load image: " + path + ". Reason: " + stbi_failure_reason());
		return false;
	}

	dataPtr = pixels;
	imageData.width = w;
	imageData.height = h;
	return true;
}

void Image::unloadImplementation()
{
	if (dataPtr != nullptr)
		stbi_image_free(dataPtr);
	
	dataPtr = nullptr;
}