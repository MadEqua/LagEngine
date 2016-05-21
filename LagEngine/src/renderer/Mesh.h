#pragma once

#include "../resources/Resource.h"
#include <vector>
#include <string>

namespace Lag
{
	class SubMesh;
	
	/*
	* Class that loads a mesh file using the Assimp library. 
	* It depends on the SubMesh class which contains all the actual data. A mesh has at least one SubMesh.
	* Usually associated with any number of Entities.
	*
	* TODO: Support animations.
	*/
	class Mesh : public Resource
	{
	public:
		Mesh(const std::string &file);
		~Mesh();

	private:
		virtual bool loadImplementation() override;
		virtual void unloadImplementation() override;

		std::vector<SubMesh*> submeshes;
	};
}