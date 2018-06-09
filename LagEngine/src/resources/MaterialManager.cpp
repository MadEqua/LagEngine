#include "MaterialManager.h"

using namespace Lag;

MaterialManager::MaterialManager(MaterialBuilder *builder) :
        XmlResourceManager("MaterialManager", builder) {
}

void MaterialManager::initializeFallbackObject() {
    fallbackObject = get("defaultMaterial");
}

MaterialBuilder::MaterialBuilder(const XmlResourceBuilderData &xmlResourceData) :
        XmlResourceBuilder(xmlResourceData) {
}

Material *MaterialBuilder::parseAndCreate(const std::string &path, const TiXmlElement &element) const {
    return new Material(path + '/' + parseFileAttribute(element));
}