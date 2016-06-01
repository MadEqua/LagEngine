#include "Texture.h"

#include "FreeImage.h"
#include "../../io/log/LogManager.h"

using namespace Lag;

Texture::Texture(const std::string &path, TextureType type, const TextureData &data) :
	Resource(path),
	type(type),
	dataPtr(nullptr)
{
}

Texture::~Texture()
{
	if (dataPtr != nullptr)
		delete[] dataPtr;
}

bool Texture::loadFromFile()
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
			"Texture", "Could not recognize format of texture: " + path);
		return false;
	}

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, path.c_str());
	//if the image failed to load, return failure
	if (!dib)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			"Texture", "Could not load texture: " + path);
		return false;
	}

	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);


	uint32 size = (FreeImage_GetBPP(dib) / 8) * width * height;
	dataPtr = new byte[size];
	memcpy(dataPtr, FreeImage_GetBits(dib), size);

	FreeImage_Unload(dib);
	return true;
}