#pragma once

#include "../../resources/Resource.h"
#include "../../Types.h"

#include <string>

namespace Lag
{
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
	};

	enum TextureComponents
	{
		LAG_TEXTURE_COMPONENTS_RGBA,
		LAG_TEXTURE_COMPONENTS_RGB,
		LAG_TEXTURE_COMPONENTS_RG,
		LAG_TEXTURE_COMPONENTS_R
	};

	enum TextureComponentType
	{
		LAG_TEXTURE_COMPONENT_TYPE_FLOAT16,
		LAG_TEXTURE_COMPONENT_TYPE_FLOAT32,
		
		LAG_TEXTURE_COMPONENT_TYPE_INT8,
		LAG_TEXTURE_COMPONENT_TYPE_INT16,
		LAG_TEXTURE_COMPONENT_TYPE_INT32,

		LAG_TEXTURE_COMPONENT_TYPE_UINT8,
		LAG_TEXTURE_COMPONENT_TYPE_UINT16,
		LAG_TEXTURE_COMPONENT_TYPE_UINT32
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
		TextureData() : components(LAG_TEXTURE_COMPONENTS_RGB),
			componentType(LAG_TEXTURE_COMPONENT_TYPE_UINT8),
			semantic(LAG_TEXTURE_SEMANTIC_DIFFUSE),
			minificationFilteringMode(LAG_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_LINEAR),
			magnificationFilteringMode(LAG_TEXTURE_FILTERING_MODE_LINEAR),
			wrappingMode{ LAG_TEXTURE_WRAPPING_MODE_REPEAT, LAG_TEXTURE_WRAPPING_MODE_REPEAT ,LAG_TEXTURE_WRAPPING_MODE_REPEAT },
			normalized(true),
			sRGB(true),
			mipmaps(8) {}
		
		TextureComponents components;
		TextureComponentType componentType;
		TextureSemantic semantic;
		TextureFilteringMode minificationFilteringMode;
		TextureFilteringMode magnificationFilteringMode;
		TexturewWrappingMode wrappingMode[3];
		bool normalized;
		bool sRGB; //is the texture in non-linar format?
		
		uint32 mipmaps;
	};
	
	//TODO: delete cpu data when sent to gpu?
	class Texture : public Resource
	{
	public:
		Texture(const std::string &path, TextureType type, const TextureData &data);
		virtual ~Texture();

		inline const TextureData& getData() const { return data; }
		inline TextureType getType() const { return type; }

		//virtual void setTextureWrap() = 0;

	protected:
		TextureType type;
		TextureData data;

		uint32 width, height, depth;
		byte *dataPtr;

		bool loadFromFile();
	};
}