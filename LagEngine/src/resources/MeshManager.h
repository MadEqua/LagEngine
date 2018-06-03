#pragma once

#include "XmlResourceManager.h"
#include "Mesh.h"
#include <string>

namespace Lag {
    class MeshBuilder : public XmlResourceBuilder<Mesh> {
    public:
        explicit MeshBuilder(const XmlResourceBuilderData &xmlResourceData);

        Mesh *parseAndCreate(const std::string &path, const TiXmlElement &element) const override;
    };


    class MeshManager : public XmlResourceManager<Mesh> {
    public:
        explicit MeshManager(MeshBuilder *builder);
        void initializeFallbackObject() override;
    };
}