#pragma once

#include "../../resources/TextureManager.h"
#include "GL4Texture.h"
#include <string>

namespace Lag
{
	class GL4TextureManager : public TextureManager
	{
	public:
		GL4TextureManager() {}
		virtual ~GL4TextureManager() {}

		virtual bool create(const std::string &name, const std::string &imageName, const TextureData &data) override
		{
			return add(name, new GL4Texture(imageName, data));
		}

		virtual bool create(const std::string &name, const std::vector<std::string> &imageNames, const TextureData &data) override
		{
			return add(name, new GL4Texture(imageNames, data));
		}
		virtual bool create(const std::string &name, const ImageData &imageData, const TextureData &textureData) override
		{
			return add(name, new GL4Texture(imageData, textureData));
		}
	};
}