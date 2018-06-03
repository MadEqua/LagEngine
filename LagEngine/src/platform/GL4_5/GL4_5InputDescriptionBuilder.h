#pragma

#include "InputDescriptionManager.h"
#include "GL4_5InputDescription.h"

namespace Lag {
    class GpuBuffer;

    class GL4_5InputDescriptionBuilder : public InputDescriptionBuilder {
    public:
        GL4InputDescription *build(const InputDescriptionMapKey &name) const override {
            return new GL4InputDescription(vertexDescription, vertexBuffer);
        }
    };
}