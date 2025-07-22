//
// Created by billy on 18/07/2025.
//

#ifndef FOOD_H
#define FOOD_H
#include <memory>
#include "SFML/System/Vector2.hpp"



class Ant;

class Food : public std::enable_shared_from_this<Food> {
public:
    explicit Food(sf::Vector2f position);

    std::weak_ptr<Ant> ant;
    bool isHeld;
    sf::Vector2f position;

    void update(float delta);
    void grab(const std::shared_ptr<Ant>& ant);
    void unload();
};



#endif //FOOD_H
