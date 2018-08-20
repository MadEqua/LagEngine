#pragma once

#include "XmlResourceManager.h"
#include "Mesh.h"

#include <string>

namespace Lag {
    class MeshBuilder : public XmlResourceBuilder<Mesh> {
    public:
        explicit MeshBuilder(const XmlResourceBuilderData &xmlResourceData);

        Mesh *build(const std::string &name) const override;
        Mesh *parseAndCreate(const std::string &path, const TiXmlElement &element) const override;

        bool fromXml;
    };


    class MeshManager : public XmlResourceManager<Mesh> {
    public:
        explicit MeshManager(MeshBuilder *builder);
        void initializeFallbackObject() override;

        //Empty mesh, to be filled programmatically.
        Handle<Mesh> getEmpty(const std::string &name);
        Handle<Mesh> get(const std::string &name); //Hiding the Manager get() on purpose.
    };
}