#pragma once

#include "Interpolator.h"

namespace Lag {

    template<typename T>
    class LinearInterpolator : public Interpolator<T> {

    public:
        explicit LinearInterpolator(const std::vector<T> &points);

    private:
        const T internalInterpolate(const T &p1, const T &p2, float t) const override;
    };

    template<typename T>
    LinearInterpolator<T>::LinearInterpolator(const std::vector<T> &points) :
        Interpolator<T>(points) {}

    template<typename T>
    const T LinearInterpolator<T>::internalInterpolate(const T &p1, const T &p2, float t) const {
        return p2 * t + p1 * (1.0f - t);
        //return p1 + t * (p2 - p1);
    }
}