#pragma once

#include <vector>

#include "../resources/Resource.h"

namespace Lag
{
	class ShaderProgram;
	
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

		virtual bool load() override;
		virtual void unload() override;
	
		inline ShaderProgram& getShaderProgram() const { return *shaderProgram; }

	private:
		std::vector<std::string> shaderStageNames;
		ShaderProgram *shaderProgram;

		bool parse();
	};
}

