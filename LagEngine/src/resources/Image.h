#pragma once

#include "../resources/XmlResource.h"
#include "../Types.h"

#include <string>

namespace Lag
{
	enum ImageComponents
	{
		LAG_IMAGE_COMPONENTS_RGBA,
		LAG_IMAGE_COMPONENTS_RGB,
		LAG_IMAGE_COMPONENTS_RG,
		LAG_IMAGE_COMPONENTS_R
	};

	enum ImageComponentType
	{
		LAG_IMAGE_COMPONENT_TYPE_FLOAT16,
		LAG_IMAGE_COMPONENT_TYPE_FLOAT32,
		
		LAG_IMAGE_COMPONENT_TYPE_INT8,
		LAG_IMAGE_COMPONENT_TYPE_INT16,
		LAG_IMAGE_COMPONENT_TYPE_INT32,

		LAG_IMAGE_COMPONENT_TYPE_UINT1,
		LAG_IMAGE_COMPONENT_TYPE_UINT4,
		LAG_IMAGE_COMPONENT_TYPE_UINT8,
		LAG_IMAGE_COMPONENT_TYPE_UINT16,
		LAG_IMAGE_COMPONENT_TYPE_UINT24,
		LAG_IMAGE_COMPONENT_TYPE_UINT32
	};

	struct ImageData
	{
		ImageData() : components(LAG_IMAGE_COMPONENTS_RGBA),
			componentType(LAG_IMAGE_COMPONENT_TYPE_UINT8),
			normalized(true),
			sRGB(true)
		{}

		uint32 width, height;
		ImageComponents components;
		ImageComponentType componentType;
		bool normalized;
		bool sRGB; //is the image in non-linar format?
	};
	
	class Image : public XmlResource
	{
	public:
		inline const ImageData& getData() const { return data; }
		inline const byte* getRawDataPointer() const { return dataPtr; }

		virtual bool loadImplementation() override;
		virtual void unloadImplementation() override;

	protected:
		friend class ImageBuilder;
		Image(const std::string &path, const ImageData &data);

		ImageData data;
		byte *dataPtr;
	};
}