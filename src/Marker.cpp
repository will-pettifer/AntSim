//
// Created by billy on 18/07/2025.
//

#include "Marker.h"

#include <algorithm>
#include <iostream>

#include "Scene.h"

Marker::Marker(const sf::Vector2f position, const Type type, const int id, const int gridId, const float lifetime) {
    this->id = id;
    this->gridId = gridId;
    this->type = type;
    this->position = position;
    this->lifetime = lifetime;
}

void Marker::update(const float delta) {
    lifetime -= delta;
    if (lifetime <= 0) {
        unload();
    }
}

void Marker::unload() {
    const auto marker = shared_from_this();
#pragma omp critical
    {
        Scene::getInstance()->markersToRemove.push_back(marker);
    }
}

