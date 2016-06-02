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
		LAG_TEXTURE_COMPONENT_TYPE_UINT32,
	};

	struct TextureData
	{
		TextureData() : components(LAG_TEXTURE_COMPONENTS_RGB),
			componentType(LAG_TEXTURE_COMPONENT_TYPE_UINT8),
			semantic(LAG_TEXTURE_SEMANTIC_DIFFUSE),
			normalized(true),
			sRGB(true),
			mipmaps(8) {}
		
		TextureComponents components;
		TextureComponentType componentType;
		TextureSemantic semantic;
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