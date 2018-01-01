#pragma once

#include "../../resources/Resource.h"
#include "../../Types.h"
#include "../../resources/Image.h"

#include <string>
#include <vector>

namespace Lag
{
	//For shader uniform matching
	enum TextureSemantic
	{
		LAG_TEXTURE_SEMANTIC_DIFFUSE,
		LAG_TEXTURE_SEMANTIC_NORMAL,
		LAG_TEXTURE_SEMANTIC_CUSTOM
	};
	
	enum TextureType
	{
		LAG_TEXTURE_TYPE_1D,
		LAG_TEXTURE_TYPE_2D,
		LAG_TEXTURE_TYPE_CUBE,
	};

	enum TextureDataType
	{
		LAG_TEXTURE_DATA_TYPE_COLOR,
		LAG_TEXTURE_DATA_TYPE_DEPTH,
		LAG_TEXTURE_DATA_TYPE_STENCIL,
		LAG_TEXTURE_DATA_TYPE_DEPTH_STENCIL
	};

	enum TextureFilteringMode
	{
		LAG_TEXTURE_FILTERING_MODE_NEAREST,
		LAG_TEXTURE_FILTERING_MODE_LINEAR,

		//[inside the mipmap]_MIPMAP_[between mipmaps]
		LAG_TEXTURE_FILTERING_MODE_NEAREST_MIPMAP_NEAREST,
		LAG_TEXTURE_FILTERING_MODE_NEAREST_MIPMAP_LINEAR,
		LAG_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_NEAREST,
		LAG_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_LINEAR
	};

	enum TexturewWrappingMode
	{
		LAG_TEXTURE_WRAPPING_MODE_REPEAT,
		LAG_TEXTURE_WRAPPING_MODE_MIRRORED_REPEAT,
		LAG_TEXTURE_WRAPPING_MODE_CLAMP_TO_EDGE,
		LAG_TEXTURE_WRAPPING_MODE_CLAMP_TO_BORDER
	};

	struct TextureData
	{
		TextureData() : 
			dataType(LAG_TEXTURE_DATA_TYPE_COLOR),
			type(LAG_TEXTURE_TYPE_2D),
			semantic(LAG_TEXTURE_SEMANTIC_DIFFUSE),
			minificationFilteringMode(LAG_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_LINEAR),
			magnificationFilteringMode(LAG_TEXTURE_FILTERING_MODE_LINEAR),
			wrappingMode{ LAG_TEXTURE_WRAPPING_MODE_REPEAT, LAG_TEXTURE_WRAPPING_MODE_REPEAT, LAG_TEXTURE_WRAPPING_MODE_REPEAT },
			mipmaps(8) {}
		
		TextureDataType dataType;
		TextureType type;
		TextureSemantic semantic;
		TextureFilteringMode minificationFilteringMode;
		TextureFilteringMode magnificationFilteringMode;
		TexturewWrappingMode wrappingMode[3];
		uint32 mipmaps;
	};

	class Image;
	struct ImageData;
	
	/*
	* Gpu representation of an Image (or a set of images). May contain mipmaps.
	*/
	class Texture : public Resource
	{
	public:
		Texture(const std::string &imageName, const TextureData &data);
		Texture(const std::vector<std::string> &imageNames, const TextureData &data);
		Texture(const ImageData &imageData, const TextureData &textureData);

		virtual ~Texture() = default;

		inline const TextureData& getTextureData() const { return textureData; }
		inline const ImageData& getImageData() const { return imageData; }

		//TODO: set texture parameter methods

	protected:
		TextureData textureData;
		ImageData imageData;

		std::vector<std::string> imageNames;
		std::vector<Image*> images;

		virtual bool loadImplementation() override;
		virtual void unloadImplementation() override;
	};
}