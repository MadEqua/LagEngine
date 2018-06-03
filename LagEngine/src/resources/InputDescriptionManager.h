#pragma once

#include <vector>
#include <unordered_map>
#include <ostream>

#include "Manager.h"
#include "InputDescription.h"

namespace Lag {
    struct InputDescriptionMapKey {
        InputDescriptionMapKey(const VertexDescription &vertexDescription, const GpuBuffer *vertexBuffer);
        bool operator==(const InputDescriptionMapKey &other) const;

        const VertexDescription vertexDescription;
        const GpuBuffer *vertexBuffer;
    };
}

namespace std {
    template<>
    struct hash<Lag::InputDescriptionMapKey> {
        std::size_t operator()(const Lag::InputDescriptionMapKey &k) const;
    };
}

namespace Lag {
    class GpuBuffer;

    class VertexDescription;

    class InputDescriptionBuilder : public IManagedObjectBuilder<InputDescriptionMapKey, InputDescription> {
    public:
        VertexDescription vertexDescription;
        Handle<GpuBuffer> vertexBuffer;
    };


    /*
    * Create and manage InputDescriptions, based on VertexDescriptions and VertexBuffers.
    * Only create new InputDescriptions when there is not one that satisfies the input.
    *
    * Concrete implementations that know how to create concrete InputDescriptions must be defined.
    */
    class InputDescriptionManager : public Manager<InputDescriptionMapKey, InputDescription> {
    public:
        explicit InputDescriptionManager(InputDescriptionBuilder *builder);

        //Convenience method for the most common operation
        Handle<InputDescription> get(const VertexDescription &vertexDescription, const Handle<GpuBuffer> vertexBuffer);

        static InputDescriptionMapKey getName(const VertexDescription &vertexDescription, const GpuBuffer *vertexBuffer);
    };
}