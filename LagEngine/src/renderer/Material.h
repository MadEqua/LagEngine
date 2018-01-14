#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "../resources/XmlResource.h"
#include "../Types.h"

namespace Lag
{
	class GpuProgram;
	class Texture;
	enum TextureSemantic;

	/*
	* A Material contains all the details for rendering an entity, except the geometry.
	*
	* TODO: list of techniques. each technique with a list of passes, each pass with shader and textures...
	* add methods to create materials by hand (no file)
	*/
	class Material : public XmlResource
	{
	public:
		inline GpuProgram& getGpuProgram() const { return *gpuProgram; }
		const std::vector<Texture*>* getTexturesBySemantic(TextureSemantic semantic) const;

		void bind() const;

	private:
		friend class MaterialManager;
		explicit Material(const std::string &filePath);
		Material(const std::vector<std::string> shaderStageNames, const std::vector<std::string> texureNames);

		virtual bool loadImplementation() override;
		virtual void unloadImplementation() override;

		bool initialize();

		std::vector<std::string> shaderStageNames;
		std::vector<std::string> textureNames;
		
		GpuProgram *gpuProgram;
		
		//Textures by associated semantic
		std::unordered_map<TextureSemantic, std::vector<Texture*>> texturesBySemantic;
		std::vector<Texture*> textures;

		bool parse();
	};
}

