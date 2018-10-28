#pragma once

#include "XmlResourceManager.h"
#include "Mesh.h"
#include "MeshRepository.h"
#include "IBoundingVolume.h"

#include <string>

namespace Lag {
    class MeshBuilder : public XmlResourceBuilder<Mesh> {
    public:
        explicit MeshBuilder(const XmlResourceBuilderData &xmlResourceData);

        Mesh *build(const std::string &name) const override;
        Mesh *parseAndCreate(const std::string &path, const TiXmlElement &element) const override;

        bool fromXml;

    protected:
        static BoundingVolumeType parseBoundingVolumeType(const TiXmlElement &element);
    };


    class MeshManager : public XmlResourceManager<Mesh> {
    public:
        explicit MeshManager(MeshBuilder *builder);
        Handle<Mesh> getFallbackObject() override;

        //Empty mesh, to be filled programmatically.
        Handle<Mesh> getEmpty(const std::string &name);
        Handle<Mesh> get(const std::string &name); //Hiding the Manager get() on purpose.

        inline Handle<Mesh> getCubeInsides() const { return meshRepository.getCubeInsides(); }
        inline Handle<Mesh> getAxisGizmo() const { return meshRepository.getAxisGizmo(); }
        inline Handle<Mesh> getAABBGizmo() const { return meshRepository.getAABBGizmo(); }
        inline Handle<Mesh> getPlaneXZ() const { return meshRepository.getPlaneXZ(); }

    private:
        MeshRepository meshRepository;
    };
}