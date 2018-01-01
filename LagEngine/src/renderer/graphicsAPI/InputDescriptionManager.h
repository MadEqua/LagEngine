#pragma once

#include <vector>
#include <unordered_map>

#include "../../core/Manager.h"
#include "InputDescription.h"
#include "../VertexDescription.h"

namespace Lag
{
	struct InputDescriptionMapKey
	{
		InputDescriptionMapKey(const VertexDescription &vd, const GpuBuffer *vb);
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

	/*
	* Create and manage InputDescriptions, based on VertexDescriptions and VertexBuffers.
	* Only create new InputDescriptions when there is not one that satisfies the input.
	*
	* Concrete implementations that know how to create concrete InputDescriptions must be defined.
	*/
	class InputDescriptionManager : public Manager<InputDescriptionMapKey, InputDescription>
	{
	public:
		InputDescriptionManager();

		//Create or get an already suitable InputDescription
		//A suitable InputDescription is one that has the same VertexDescription for the same GpuBuffer
		InputDescription* createInputDescription(const VertexDescription &vertexDescription, const GpuBuffer &vertexBuffer);

	protected:
		virtual InputDescription* internalCreateInputDescription(const VertexDescription &vertexDescription, const GpuBuffer &vertexBuffer) = 0;
	};
}