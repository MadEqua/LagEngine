#pragma once

#include "../../resources/TextureManager.h"
#include "GL4Texture.h"
#include <string>

namespace Lag
{
	class GL4TextureManager : public TextureManager
	{
	public:
		virtual Texture* internalCreate(const std::string &imageName, const TextureData &data) override
		{
			return new GL4Texture(imageName, data);
		}

		virtual Texture* internalCreate(const std::vector<std::string> &imageNames, const TextureData &data) override
		{
			return new GL4Texture(imageNames, data);
		}

		virtual Texture* internalCreate(const ImageData &imageData, const TextureData &textureData) override
		{
			return new GL4Texture(imageData, textureData);
		}
	};
}