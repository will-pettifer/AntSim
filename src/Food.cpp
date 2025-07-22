//
// Created by billy on 18/07/2025.
//

#include "Food.h"

#include <iostream>

#include "Ant.h"
#include "Scene.h"


Food::Food(const sf::Vector2f position) {
    this->position = position;
    isHeld = false;
}

void Food::update(float delta) {
    if (isHeld) {
        if (const auto antPtr = ant.lock()) {
            position = antPtr->position;
            cout << "working" << endl;
        } else {
            isHeld = false;
            unload();
        }
    }
}

void Food::grab(const std::shared_ptr<Ant>& ant) {
    isHeld = true;
    this->ant = ant;

    const auto self = shared_from_this();
    Scene::getInstance()->heldFoods.push_back(self);

    const auto pos = static_cast<sf::Vector2u>(position / 10.f);
    auto& cell = Scene::getInstance()->foodGrid[pos.x][pos.y];

    for (auto it = cell.begin(); it != cell.end(); ++it) {
        if (*it == self) {
            cell.erase(it);
            break;
        }
    }
}

void Food::unload() {
    if (const auto antPtr = ant.lock()) {
        antPtr->heldFood = nullptr;
    }
    ant.reset();
    isHeld = false;

    Scene::getInstance()->foodsToRemove.push_back(shared_from_this());
}
