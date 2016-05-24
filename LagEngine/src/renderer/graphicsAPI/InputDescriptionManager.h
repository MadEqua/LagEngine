#pragma once

#include <vector>
#include <unordered_map>

namespace Lag
{
	class InputDescription;
	class VertexData;
	class GpuBuffer;
	class VertexDescription;
	
	/*
	* Create and manage InputDescriptions, based on VertexDescriptions and VertexBuffers.
	* Only create new InputDescriptions when there is not one that satisfies the input.
	* Also creates and manages VertexDescriptions.
	*
	* Concrete implementations that know how to create concrete InputDescriptions must be defined.
	*/
	class InputDescriptionManager
	{
	public:
		InputDescriptionManager();
		virtual ~InputDescriptionManager();

		//Create or get an already suitable InputDescription
		//A suitable InputDescription is one that describes correctly the input VertexData
		InputDescription* getInputDescription(const VertexDescription &vertexDescription,
			const GpuBuffer &vertexBuffer);

		VertexDescription& createVertexDescription();

	protected:

		virtual InputDescription* createInputDescription(const VertexDescription &vertexDescription, 
			const GpuBuffer &vertexBuffer) = 0;

		
		//We should return the same InputDescription if VertexData refers
		//the same VertexBuffer, with a similar VertexDescription.
		//(Basically ignore the offsets from a VertexData)
		class MapKey
		{
		public:
			MapKey(const VertexDescription *vd, const GpuBuffer *vb);
			~MapKey();

			bool operator==(const MapKey &other) const;

			struct MapKeyHasher
			{
				std::size_t operator()(const MapKey& k) const;
			};

		private:
			const VertexDescription *vertexDescription;
			const GpuBuffer *vertexBuffer;
		};

		std::unordered_map<MapKey, InputDescription*, MapKey::MapKeyHasher> inputDescriptions;

		std::vector<VertexDescription*> vertexDescriptions;
	};
}