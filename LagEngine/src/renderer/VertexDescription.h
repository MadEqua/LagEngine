#pragma once

#include "../Types.h"

#include <vector>

namespace Lag
{
	enum VertexAttributeSemantic
	{
		LAG_VX_ATTR_SEMANTIC_POSITION,
		LAG_VX_ATTR_SEMANTIC_COLOR,
		LAG_VX_ATTR_SEMANTIC_TEX_COORD,
		LAG_VX_ATTR_SEMANTIC_NORMAL,
		LAG_VX_ATTR_SEMANTIC_BINORMAL,
		LAG_VX_ATTR_SEMANTIC_TANGENT
	};

	enum VertexAttributeType
	{
		LAG_VX_ATTR_TYPE_HALF_FLOAT,
		LAG_VX_ATTR_TYPE_FLOAT,
		LAG_VX_ATTR_TYPE_FIXED, //16-bit fixed-point two's complement value
		LAG_VX_ATTR_TYPE_INT8,
		LAG_VX_ATTR_TYPE_UINT8,
		LAG_VX_ATTR_TYPE_INT16,
		LAG_VX_ATTR_TYPE_UINT16,
		LAG_VX_ATTR_TYPE_INT32,
		LAG_VX_ATTR_TYPE_UINT32
	};

	/*
	* An attribute of a vertex. Contained on a VertexDescription.
	*/
	class VertexAttribute
	{
	public:
		inline VertexAttributeSemantic getSemantic() const { return semantic; }
		inline uint32 getOffset() const { return offset; }
		inline uint8 getLength() const { return length; }
		inline VertexAttributeType getType() const { return type; }
		inline uint8 getIndex() const { return index; }
		inline bool getIsNormalized() const { return isNormalized; }

		uint16 getByteSize() const;
		static uint8 getTypeByteSize(VertexAttributeType type);

		bool operator==(const VertexAttribute &other) const;
		operator std::size_t() const;

		~VertexAttribute();

	private:
		//Only VertexDescriptions can construct VertexAttributes
		friend class VertexDescription;

		VertexAttribute(VertexAttributeSemantic semantic, uint32 offset, uint8 length,
			VertexAttributeType type, uint8 index, bool isNormalized);

		VertexAttributeSemantic semantic;
		uint32 offset; //How far the attribute is offset from the start of a vertex (bytes)
		uint8 length; //Ex: a position usually has length 3 (x,y,z)
		VertexAttributeType type;
		uint8 index; //Allowing several attributes with the same semantic (ex: texCoord1, texCoord2)
		bool isNormalized;
	};

	
	/*
	* Describes a vertex stored on a vertex buffer. (All the attributes of a vertex)
	*/
	class VertexDescription
	{
	public:
		inline uint32 getAttributeCount() const { return attributes.size(); }
		inline const std::vector<VertexAttribute>& getAttributes() const { return attributes; }

		void addAttribute(VertexAttributeSemantic semantic, uint32 offset, uint8 length, VertexAttributeType type, uint8 index = 0, bool isNormalized = false);

		bool operator==(const VertexDescription &other) const;
		operator std::size_t() const;

	private:
		std::vector<VertexAttribute> attributes;

		//Only GpuBufferManager can construct VertexDescriptions
		friend class GpuBufferManager;
		VertexDescription();
		~VertexDescription();
	};
}