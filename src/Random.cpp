//
// Created by billy on 19/07/2025.
//

#include "Random.h"
#include <random>


float Random::Float(const float min, const float max) {
    return min + unitDist(gen) * (max - min);
}

sf::Vector2f Random::OnUnitCircle() {
    const float angle = unitDist(gen) * 2 * sf::priv::pi;
    return {
        std::cos(angle),
        std::sin(angle)
    };
}

std::random_device Random::rd;
std::mt19937 Random::gen(Random::rd());
std::uniform_real_distribution<float> Random::unitDist(0.f, 1.f);