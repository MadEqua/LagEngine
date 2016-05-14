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
		ShaderProgram *shaderProgram;
	};
}

