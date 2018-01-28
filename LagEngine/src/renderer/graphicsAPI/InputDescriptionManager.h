#pragma once

#include <vector>
#include <unordered_map>
#include <ostream>

#include "GpuBuffer.h"
#include "../../core/Manager.h"
#include "InputDescription.h"
#include "../VertexDescription.h"

namespace Lag
{
	struct InputDescriptionMapKey
	{
		InputDescriptionMapKey(const VertexDescription &vertexDescription, const GpuBuffer *vertexBuffer);
		bool operator==(const InputDescriptionMapKey &other) const;

		const VertexDescription vertexDescription;
		const GpuBuffer *vertexBuffer;
	};
}

namespace std 
{
	template <>
	struct hash<Lag::InputDescriptionMapKey>
	{
		std::size_t operator()(const Lag::InputDescriptionMapKey& k) const;
	};
}

namespace Lag
{
	class GpuBuffer;
	class VertexDescription;

	class InputDescriptionBuilder : public IManagedObjectBuilder<InputDescriptionMapKey, InputDescription>
	{
	public:
		VertexDescription vertexDescription;
		GpuBuffer *vertexBuffer;
	};


	/*
	* Create and manage InputDescriptions, based on VertexDescriptions and VertexBuffers.
	* Only create new InputDescriptions when there is not one that satisfies the input.
	*
	* Concrete implementations that know how to create concrete InputDescriptions must be defined.
	*/
	class InputDescriptionManager : public Manager<InputDescriptionMapKey, InputDescription>
	{
	public:
		explicit InputDescriptionManager(InputDescriptionBuilder *builder);

		//Convenience methods for the most common operations
		InputDescription* get(const VertexDescription &vertexDescription, const GpuBuffer *vertexBuffer);
		InputDescription* get(const VertexDescription &vertexDescription, const GpuBuffer *vertexBuffer, ManagedObject &parent);
		
		static InputDescriptionMapKey getName(const VertexDescription &vertexDescription, const GpuBuffer *vertexBuffer);
	};
}