#pragma

#include "InputDescriptionManager.h"
#include "GL4_5InputDescription.h"

namespace Lag {
    class GpuBuffer;

    class GL4_5InputDescriptionBuilder : public InputDescriptionBuilder {
    public:
        GL4_5InputDescription *build(const InputDescriptionMapKey &name) const override {
            return new GL4_5InputDescription(vertexDescription, vertexBuffer);
        }
    };
}