#pragma once

#include "../Types.h"

#include <vector>

namespace Lag
{
	enum VertexAttributeSemantic
	{
		POSITION,
		COLOR,
		TEX_COORD,
		NORMAL,
		BINORMAL,
		TANGENT
	};

	enum VertexAttributeType
	{
		HALF_FLOAT,
		FLOAT,
		FIXED, //16-bit fixed-point two's complement value
		INT8,
		UINT8,
		INT16,
		UINT16,
		INT32,
		UINT32
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

	private:
		std::vector<VertexAttribute> attributes;

		//Only GpuBufferManager can construct VertexDescriptions
		friend class GpuBufferManager;
		VertexDescription();
		~VertexDescription();
	};
}