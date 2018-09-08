#include "MeshManager.h"
#include "Root.h"
#include "MeshManager.h"

using namespace Lag;

MeshBuilder::MeshBuilder(const XmlResourceBuilderData &xmlResourceData) :
        XmlResourceBuilder(xmlResourceData) {
}

Mesh *MeshBuilder::build(const std::string &name) const {
    if(fromXml)
        return XmlResourceBuilder::build(name);
    else
        return new Mesh();
}

Mesh *MeshBuilder::parseAndCreate(const std::string &path, const TiXmlElement &element) const {
    return new Mesh(path + '/' + parseFileAttribute(element));
}

MeshManager::MeshManager(MeshBuilder *builder) :
        XmlResourceManager("MeshManager", builder),
        meshRepository(*this) {
}

Handle<Mesh> MeshManager::getFallbackObject() {
    return get("cube");
}

Handle<Mesh> MeshManager::getEmpty(const std::string &name) {
    (dynamic_cast<MeshBuilder*>(builder))->fromXml = false;
    return Manager::get(name);
}

Handle<Mesh> MeshManager::get(const std::string &name) {
    (dynamic_cast<MeshBuilder*>(builder))->fromXml = true;
    return Manager::get(name);
}
