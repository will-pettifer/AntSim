//
// Created by billy on 18/07/2025.
//

#include "Ant.h"

#include <iostream>

#include "Helpers.h"
#include "Random.h"
#include "Scene.h"
#include "Sensor.h"
#include "SFML/Graphics/RenderWindow.hpp"


Ant::Ant(const sf::Vector2f position) {
    this->position = position;
    direction = Random::OnUnitCircle();
    velocity = Random::OnUnitCircle();
    state = FOOD;
    timer = Random::Float(0, MARKER_INTERVAL);
    frameTimer = Random::Float(0, UPDATE_INTERVAL);
}


void Ant::update(const float delta) {
    frameTimer += delta;
    if (frameTimer > UPDATE_INTERVAL) {
        frameTimer -= UPDATE_INTERVAL;
        direction = (
            Navigate().normalized() +
            Random::OnUnitCircle() *
            WANDER_STRENGTH
            ).normalized();
    }
    timer += delta;
    if (timer > MARKER_INTERVAL) {
        timer -= MARKER_INTERVAL;

        if (state == HOME) Scene::getInstance()->loadFoodMarker(position);
        else if (state == FOOD) Scene::getInstance()->loadHomeMarker(position);
    }

    const sf::Vector2f targetVelocity = direction * SPEED;
    const sf::Vector2f targetSteer = (targetVelocity - velocity) * STEER_STRENGTH;
    const sf::Vector2f acceleration = Helpers::ClampVector(targetSteer, STEER_STRENGTH);

    velocity = Helpers::ClampVector(velocity + acceleration * delta, SPEED);
    position += velocity * delta;
}


sf::Vector2f Ant::Navigate() {
    if (position.x > 1030) direction = {-1, 0};
    else if (position.x < 250) direction = {1, 0};
    if (position.y > 670) direction = {0, -1};
    else if (position.y < 50) direction = {0, 1};

    Sensor sensor(position, direction);
    const auto senseArea = sensor.GetSenseArea();

    if (sensor.ObstacleSense(senseArea)) {
        direction = Helpers::RotateVector(direction, sf::priv::pi);
        velocity = {0, 0};
        return direction;
    }
    if (state == FOOD) {

        #pragma omp critical
        {
            if (const shared_ptr<Food> food = sensor.SenseFood(senseArea); food != nullptr) {
                state = HOME;
                heldFood = food;
                food->grab(shared_from_this());
            }
        }

        if (heldFood != nullptr) {
            FoodBomb();
            direction = Helpers::RotateVector(direction, sf::priv::pi);
            velocity = {0, 0};
            return direction;
        }
        return sensor.MultiSense(Scene::getInstance()->foodMarkerGrid, senseArea, direction);
    }
    if (state == HOME) {

        if ((sf::Vector2f(1280.f / 2, 720.f / 2) - position).lengthSquared() < 1000.f) {
            state = FOOD;
            heldFood->unload();

            #pragma omp critical
            {
                Scene::getInstance()->score++;
            }

            HomeBomb();
            direction = Helpers::RotateVector(direction, sf::priv::pi);
            return direction;
        }
        return sensor.MultiSense(Scene::getInstance()->homeMarkerGrid, senseArea, direction);
    }
    return direction;
}

void Ant::FoodBomb() const {
    const auto scene = Scene::getInstance();
    const auto velNorm = Helpers::Normalize(velocity);
    for (int i = 0; i < BOMB_COUNT; ++i) {
        scene->loadFoodMarker(position + velNorm * 20.f + Random::OnUnitCircle() * Random::Float(BOMB_RANGE_MIN, BOMB_RANGE_MAX));
        scene->loadFoodMarker(position + velNorm * 20.f + Random::OnUnitCircle() * Random::Float(BOMB_RANGE_MAX, BOMB_RANGE_OUTER));
    }
}

void Ant::HomeBomb() const {
    const auto scene = Scene::getInstance();
    for (int i = 0; i < BOMB_COUNT; ++i) {
        scene->loadHomeMarker(position + Random::OnUnitCircle() * Random::Float(BOMB_RANGE_MIN, BOMB_RANGE_MAX));
        scene->loadHomeMarker(position + Random::OnUnitCircle() * Random::Float(BOMB_RANGE_MAX, BOMB_RANGE_OUTER));
    }
}

