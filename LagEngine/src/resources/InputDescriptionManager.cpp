#include "InputDescriptionManager.h"

using namespace Lag;

InputDescriptionMapKey::InputDescriptionMapKey(const VertexDescription &vertexDescription,
                                               const GpuBuffer *vertexBuffer) :
        vertexDescription(vertexDescription), vertexBuffer(vertexBuffer) {
}

bool InputDescriptionMapKey::operator==(const InputDescriptionMapKey &other) const {
    return vertexBuffer == other.vertexBuffer && vertexDescription == other.vertexDescription;
}


std::size_t std::hash<Lag::InputDescriptionMapKey>::operator()(const Lag::InputDescriptionMapKey &k) const {
    return std::hash<const GpuBuffer *>()(k.vertexBuffer) ^ static_cast<std::size_t>(k.vertexDescription);
}


InputDescriptionManager::InputDescriptionManager(InputDescriptionBuilder *builder) :
        Manager("InputDescriptionManager", builder) {
}

Handle<InputDescription> InputDescriptionManager::get(const VertexDescription &vertexDescription, const Handle<GpuBuffer> vertexBuffer) {
    auto &inputDescriptionBuilder = dynamic_cast<InputDescriptionBuilder &>(*builder);
    inputDescriptionBuilder.vertexBuffer = vertexBuffer;
    inputDescriptionBuilder.vertexDescription = vertexDescription;
    Handle<InputDescription> handle = Manager::get(getName(vertexDescription, vertexBuffer.get()));
    inputDescriptionBuilder.vertexBuffer.invalidate(); //release Handle on the builder, the created InputDescription will own it
    return handle;
}

InputDescriptionMapKey InputDescriptionManager::getName(const VertexDescription &vertexDescription, const GpuBuffer *vertexBuffer) {
    return InputDescriptionMapKey(vertexDescription, vertexBuffer);
}