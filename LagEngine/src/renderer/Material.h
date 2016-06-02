#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "../resources/Resource.h"
#include "../Types.h"
#include "GpuProgramUniformDescription.h"

namespace Lag
{
	class GpuProgram;
	class Texture;
	enum TextureSemantic;
	class Renderer;

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
		const std::vector<Texture*>* getTexturesBySemantic(TextureSemantic semantic) const;

		void bind() const;

	private:
		virtual bool loadImplementation() override;
		virtual void unloadImplementation() override;

		std::vector<std::string> shaderStageNames;
		GpuProgram *gpuProgram;

		Renderer &renderer;
		
		//Textures by associated semantic
		std::unordered_map<TextureSemantic, std::vector<Texture*>> texturesBySemantic;
		std::vector<Texture*> textures;

		bool parse();
	};
}

