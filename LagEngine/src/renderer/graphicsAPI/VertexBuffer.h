#pragma once

#include "../../Types.h"

namespace Lag
{
	//The number of possible VertexAttributeSemantics
	const static int ATTRIBUTE_SEMANTIC_COUNT = 6;
	enum VertexAttributeSemantic
	{
		POSITION,
		COLOR,
		TEXCOORD1,
		TEXCOORD2,
		NORMAL,
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
	

	class VertexBufferDetails
	{
		friend class VertexBuffer;

	public:
		VertexBufferDetails(uint32 vertexCount, bool interleavedVertices);
		~VertexBufferDetails();

		//Enable an attribute (color, normal, texcoord, ...) 
		void enableAttribute(VertexAttributeSemantic semantic, uint8 length, VertexAttributeType type, bool isNormalized = false);

	private:

		//Interleaved ex: POS1,COLOR1,TEXCOORD1,POS2,COLOR2,TEXCOORD2->XYZRGBSTXYZRGBST
		//Non-interleaved ex: POS1,PO2,COLOR1,COLOR2,TEXCOORD1,TEXCORD2->XYZXYZRGBRGBSTST
		bool interleavedVertices;

		uint32 vertexCount;

		//All the next vectors are indexed by VertexAttributeSemantic types
		//Which attributes this buffer contains
		bool enabledAttributes[ATTRIBUTE_SEMANTIC_COUNT];

		//The length of each attribute (ex: a 3D position has a length of 3)
		uint8 attributeLengths[ATTRIBUTE_SEMANTIC_COUNT];

		//The type of each attribute
		VertexAttributeType attributeTypes[ATTRIBUTE_SEMANTIC_COUNT];

		//If normalized​, the value will be converted to a float via integer normalization 
		//(an unsigned byte value of 255 becomes 1.0f)
		bool attributeIsNormalized[ATTRIBUTE_SEMANTIC_COUNT];

		//Below values are measured in bytes
		//Space between the buffer start and the first attribute value
		uint16 attributeOffsets[ATTRIBUTE_SEMANTIC_COUNT];

		//Space between the same vertex attribute in consecutive vertices
		uint16 attributeStrides[ATTRIBUTE_SEMANTIC_COUNT];

		void computeOffsetAndStride(VertexAttributeSemantic semantic);
		uint16 computeVertexBytesSize() const;
		uint8 computeAttributeBytesSize(VertexAttributeSemantic semantic) const;
		uint8 getAttributeTypeBytesSize(VertexAttributeType type) const;
	};
	
	//////////////////////////////////////////////
	//////////////////////////////////////////////

	/*
	* For now it assumes a static, constant size sent on VertexBufferDetails.
	*/
	class VertexBuffer
	{
	public:

		//Expected to use setVertexAttribute() to fill the buffer with vertex data
		VertexBuffer(const VertexBufferDetails &details);
		
		//Will copy the received data, and VertexBufferDetails. Safe to delete the sent in pointer.
		VertexBuffer(const VertexBufferDetails &details, const byte *data);
		
		virtual ~VertexBuffer();

		//Fills the buffer for 1 vertex attribute. Ex: Vertex 4 Normal = data;
		void setVertexAttributeData(VertexAttributeSemantic attribute, uint32 vertexNumber, byte* data);

		//virtual void bind() = 0;
		virtual void sendToGpu() = 0;

	protected:
		VertexBufferDetails details;
		byte *data;
		uint32 dataSize;
	};
}