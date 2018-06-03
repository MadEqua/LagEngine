#pragma once

#include "RenderTargetManager.h"
#include "GL4_5RenderToTexture.h"

namespace Lag {
    class GL4_5RenderTargetBuilder : public RenderTargetBuilder {
    public:
        RenderTarget *build(const uint32 &name) const override {
            return new GL4RenderToTexture(width, height, renderPhase);
        }
    };
}