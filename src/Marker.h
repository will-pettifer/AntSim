//
// Created by billy on 18/07/2025.
//

#ifndef MARKER_H
#define MARKER_H
#include <memory>

#include "SFML/System/Vector2.hpp"


class Marker : public std::enable_shared_from_this<Marker> {
public:
    explicit Marker(sf::Vector2f position, float lifetime = LIFETIME);

    static constexpr float LIFETIME = 60.0f;

    sf::Vector2f position;
    float lifetime;

    void update(float delta);
    void unload();
};



#endif //MARKER_H
