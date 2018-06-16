#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "Handle.h"
#include "Texture.h"
#include "XmlResource.h"
#include "Types.h"
#include "DepthSettings.h"
#include "BlendingSettings.h"

class TiXmlElement;

namespace Lag {
    class GpuProgram;
    class Texture;
    enum class TextureSemantic : uint8;

    /*
    * A Material contains all the details for rendering an entity, except the geometry.
    *
    * TODO: list of techniques. each technique with a list of passes, each pass with shader and textures...
    * add methods to create materials by hand (no file)
    */
    class Material : public XmlResource {
    public:
        inline GpuProgram& getGpuProgram() const { return *gpuProgram; }

        void bind() const;

        const std::vector<Texture *>* getTexturesBySemantic(TextureSemantic semantic) const;
        inline const DepthSettings& getDepthSettings() const { return depthSettings; }
        inline const BlendingSettings& getBlendingSettings() const { return blendingSettings; }

    private:
        friend class MaterialBuilder;

        explicit Material(const std::string &filePath);
        //Material(const std::vector<std::string> &shaderStageNames, const std::vector<std::string> &textureNames);

        bool loadImplementation() override;
        void unloadImplementation() override;

        bool initialize();

        std::vector<std::string> shaderStageNames;
        std::vector<std::string> textureNames;

        DepthSettings depthSettings;
        BlendingSettings blendingSettings;

        Handle<GpuProgram> gpuProgram;

        //Textures by associated semantic
        std::unordered_map<TextureSemantic, std::vector<Texture *>> texturesBySemantic;
        std::vector<Handle<Texture>> textures;

        bool parse();
        bool parseBlendingSettings(const TiXmlElement *blendingNode);

        static BlendingFunction parseBlendingFunction(const std::string &function);
        static BlendingEquation parseBlendingEquation(const std::string &equation);
    };
}

