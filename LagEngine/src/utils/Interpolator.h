#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace Lag {

    template <typename T>
    class Interpolator {

    public:
        explicit Interpolator(const std::vector<T> &points);

        //t will loop inside the [0, 1] interval.
        const T interpolate(float t) const;

        //t will loop inside the [0, tLimit] interval.
        const T interpolate(float t, float tLimit) const;

    private:
        std::vector<T> points;
        virtual const T internalInterpolate(const T &p1, const T &p2, float t) const = 0;
    };

    template<typename T>
    Interpolator<T>::Interpolator(const std::vector<T> &points) :
        points(points) {}

    template<typename T>
    const T Interpolator<T>::interpolate(float t) const {
        float loopedT = t < 0.0f ? 
            1.0f + (t + glm::ceil(t)) :
            t - glm::floor(t);

        float segmentCount = glm::max(points.size() - 1.0f, 1.0f);

        int p1 = static_cast<int>(glm::floor(loopedT * segmentCount));
        int p2 = (p1 + 1) % points.size();

        float step = 1.0f / segmentCount;
        float p1t = p1 * step;
        float segmentT = (loopedT - p1t) / step;

        return internalInterpolate(points[p1], points[p2], segmentT);
    }

    template<typename T>
    const T Interpolator<T>::interpolate(float t, float tLimit) const {
        return interpolate(t / tLimit);
    }
}

