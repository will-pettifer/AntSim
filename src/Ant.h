//
// Created by billy on 18/07/2025.
//

#ifndef ANT_H
#define ANT_H
#include <memory>

#include "Food.h"
#include "SFML/System/Vector2.hpp"

class Ant : public std::enable_shared_from_this<Ant> {
private:

    static constexpr float SPEED = 25.f;
    static constexpr float STEER_STRENGTH = 50.f;
    static constexpr float WANDER_STRENGTH = 0.2f;

    static constexpr float UPDATE_INTERVAL = 0.05f;
    static constexpr float MARKER_INTERVAL = 0.2f;

    static constexpr uint8_t BOMB_COUNT = 20;
    static constexpr float BOMB_RANGE_MIN = 0.f;
    static constexpr float BOMB_RANGE_MAX = 7.f;
    static constexpr float BOMB_RANGE_OUTER = 15.f;



    float frameTimer = 0.f;
    float timer = 0.f;

    sf::Vector2f Navigate();
    void FoodBomb() const;
    void HomeBomb() const;
public:
    explicit Ant(sf::Vector2f position = sf::Vector2f(0.0f, 0.0f));

    enum State{ HOME, FOOD};

    State state;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f direction;
    std::shared_ptr<Food> heldFood;

    void update(float delta);
};



#endif //ANT_H
