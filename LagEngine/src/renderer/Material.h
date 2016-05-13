#pragma once

#include <vector>

namespace Lag
{
	class ShaderProgram;
	
	/*
	* A Material contains all the details for rendering an entity, except the geometry.
	*
	* TODO: list of techniques. each technique with a list of passes, each pass with shader and textures...
	*/
	class Material
	{
	public:
		Material(ShaderProgram &shaderProgram);
		~Material();
	
		inline ShaderProgram& getShaderProgram() const { return shaderProgram; }

	private:
		ShaderProgram &shaderProgram;
	};
}

