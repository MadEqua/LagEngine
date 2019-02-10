#pragma once

#include "Interpolator.h"

namespace Lag {

    template<typename T>
    class CubicHermiteInterpolator : public Interpolator<T> {

    public:
        explicit CubicHermiteInterpolator(const std::vector<T> &points);

    private:
        const T internalInterpolate(const T &p1, const T &p2, float t) const override;
    };

    template<typename T>
    CubicHermiteInterpolator<T>::CubicHermiteInterpolator(const std::vector<T> &points) :
        Interpolator<T>(points) {}

    template<typename T>
    const T CubicHermiteInterpolator<T>::internalInterpolate(const T &p1, const T &p2, float t) const {
        float newT = glm::clamp(t * t * (3.0f - 2.0f * t), 0.0f, 1.0f);
        return p1 * (1.0f - newT) + p2 * newT;
    }
}