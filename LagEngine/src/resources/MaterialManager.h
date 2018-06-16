#pragma once

#include "XmlResourceManager.h"
#include "Material.h"

namespace Lag {
    class MaterialBuilder : public XmlResourceBuilder<Material> {
    public:
        explicit MaterialBuilder(const XmlResourceBuilderData &xmlResourceData);

        Material *parseAndCreate(const std::string &path, const TiXmlElement &element) const override;
    };

    class MaterialManager : public XmlResourceManager<Material> {
    public:
        explicit MaterialManager(MaterialBuilder *builder);
        void initializeFallbackObject() override;
    };
}