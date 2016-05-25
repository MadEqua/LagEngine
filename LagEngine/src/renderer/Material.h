#pragma once

#include <vector>
#include <string>

#include "../resources/Resource.h"

namespace Lag
{
	class GpuProgram;
	
	/*
	* A Material contains all the details for rendering an entity, except the geometry.
	*
	* TODO: list of techniques. each technique with a list of passes, each pass with shader and textures...
	* add methods to create materials by hand (no file)
	*/
	class Material : public Resource
	{
	public:
		Material(const std::string &filePath);
		virtual ~Material();
	
		inline GpuProgram& getGpuProgram() const { return *gpuProgram; }

	private:
		virtual bool loadImplementation() override;
		virtual void unloadImplementation() override;

		std::vector<std::string> shaderStageNames;
		GpuProgram *gpuProgram;

		bool parse();
	};
}

