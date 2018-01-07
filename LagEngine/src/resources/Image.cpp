#include "Image.h"

#include "FreeImage.h"
#include "../io/log/LogManager.h"

using namespace Lag;

Image::Image(const std::string &path, const ImageData &data) :
	XmlResource(path),
	data(data),
	dataPtr(nullptr)
{
}

bool Image::loadImplementation()
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	FIBITMAP *dib = nullptr;

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(path.c_str(), 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(path.c_str());

	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			"Image", "Could not recognize format of image: " + path);
		return false;
	}

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, path.c_str());
	//if the image failed to load, return failure
	if (!dib)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			"Image", "Could not load image: " + path);
		return false;
	}

	data.width = FreeImage_GetWidth(dib);
	data.height = FreeImage_GetHeight(dib);

	uint32 size = (FreeImage_GetBPP(dib) / 8) * data.width * data.height;
	dataPtr = new byte[size];
	memcpy(dataPtr, FreeImage_GetBits(dib), size);

	FreeImage_Unload(dib);
	return true;
}

void Image::unloadImplementation()
{
	if (dataPtr != nullptr)
		delete[] dataPtr;
	
	dataPtr = nullptr;
}