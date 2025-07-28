//
// Created by billy on 18/07/2025.
//

#ifndef MARKER_H
#define MARKER_H
#include <memory>

#include "SFML/System/Vector2.hpp"


class Marker : public std::enable_shared_from_this<Marker> {
public:
    enum Type{Home, Food};

    static constexpr float LIFETIME = 40.0f;

    explicit Marker(sf::Vector2f position, Type type, int id = 0, int gridId = 0, float lifetime = LIFETIME);

    Type type;

    sf::Vector2f position;
    float lifetime;
    int id;
    int gridId;

    void update(float delta);
    void unload();
};



#endif //MARKER_H
