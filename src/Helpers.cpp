//
// Created by billy on 19/07/2025.
//

#include "Helpers.h"

#include <chrono>
#include <cmath>
#include <iostream>


sf::Vector2f Helpers::RotateVector(const sf::Vector2f v, float a) {
    const float sinA = sinf(a);
    const float cosA = cosf(a);
    return {
        v.x * cosA - v.y * sinA,
        v.x * sinA + v.y * cosA
    };
}

sf::Vector2f Helpers::ClampVector(const sf::Vector2f vector, const float limit) {
    if (const float lengthSquared = vector.lengthSquared(); lengthSquared > limit * limit) {
        const float ratio = limit / sqrtf(lengthSquared);
        return vector * ratio;
    };
    return vector;
}

sf::Vector2f Helpers::Normalize(const sf::Vector2f vector) {
    if (vector.lengthSquared() == 0) return {0, 0};
    return vector.normalized();
}