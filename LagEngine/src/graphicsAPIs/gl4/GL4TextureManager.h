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

		virtual bool create(const std::string &name, const std::string &file,
			TextureType type, const TextureData &data) override
		{
			return add(name, new GL4Texture(file, type, data));
		}
	};
}