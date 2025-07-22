//
// Created by billy on 19/07/2025.
//

#ifndef RANDOM_H
#define RANDOM_H
#include <random>

#include "SFML/System/Vector2.hpp"


class Random {
private:
    static std::random_device rd;
    static std::mt19937 gen;
    static std::uniform_real_distribution<float> unitDist;
public:
    static float Float(float min, float max);
    static sf::Vector2f OnUnitCircle();
};



#endif //RANDOM_H
