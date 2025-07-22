//
// Created by billy on 19/07/2025.
//

#ifndef HELPERS_H
#define HELPERS_H
#include "SFML/System/Vector2.hpp"
#include <ostream>

inline std::ostream& operator<<(std::ostream& os, const sf::Vector2f& v) {
    return os << "(" << v.x << ", " << v.y << ")";
}


class Helpers {
public:
    static sf::Vector2f RotateVector(sf::Vector2f vector, float angle);
    static sf::Vector2f ClampVector(sf::Vector2f vector, float limit);
    static sf::Vector2f Normalize(sf::Vector2f vector);
};



#endif //HELPERS_H
