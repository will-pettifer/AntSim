//
// Created by billy on 20/07/2025.
//

#include "Sensor.h"

#include "Food.h"
#include "Helpers.h"
#include "Scene.h"


Sensor::Sensor(const sf::Vector2f position, const sf::Vector2f direction) {
    this->position = position;
    this->direction = direction;

    collider0 = position + direction * SENSE_DISTANCE;
    collider1 = position + Helpers::RotateVector(direction, SENSE_ANGLE) * SENSE_DISTANCE;
    collider2 = position + Helpers::RotateVector(direction, -SENSE_ANGLE) * SENSE_DISTANCE;
    count0 = 0;
    count1 = 0;
    count2 = 0;
}

std::vector<sf::Vector2u> Sensor::GetSenseArea() const {
    std::vector<sf::Vector2u> area;
    const auto coord = sf::Vector2u(position / 10.f);
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            const sf::Vector2u celli = {coord.x + i, coord.y + j};
            const auto cell = static_cast<sf::Vector2f>(celli) * 10.f;
            if ((collider0.x + SENSE_RADIUS > cell.x && collider0.x - SENSE_RADIUS < cell.x + 10.f
                && collider0.y + SENSE_RADIUS > cell.y && collider0.y - SENSE_RADIUS < cell.y + 10.f)
                || (collider1.x + SENSE_RADIUS > cell.x && collider1.x - SENSE_RADIUS < cell.x + 10.f
                && collider1.y + SENSE_RADIUS > cell.y && collider1.y - SENSE_RADIUS < cell.y + 10.f)
                || (collider2.x + SENSE_RADIUS > cell.x && collider2.x - SENSE_RADIUS < cell.x + 10.f
                && collider2.y + SENSE_RADIUS > cell.y && collider2.y - SENSE_RADIUS < cell.y + 10.f)) {
                area.push_back(celli);
            }
        }
    }
    return area;
}

sf::Vector2f Sensor::MultiSense(
    const std::vector<std::shared_ptr<Marker>> markers[128][72], const std::vector<sf::Vector2u>& area, const sf::Vector2f def) {

    for (auto& cell : area) {
        for (const auto& ptr : markers[cell.x][cell.y]) {
            if (const sf::Vector2f pos = ptr->position; (collider0 - pos).lengthSquared() < SENSE_RADIUS_SQR) { count0++; }
            else if ((collider1 - pos).lengthSquared() < SENSE_RADIUS_SQR) { count1++; }
            else if ((collider2 - pos).lengthSquared() < SENSE_RADIUS_SQR) { count2++; }
        }
    }

    if (count0 > 100) return def;
    if (count0 >= count1 && count0 >= count2) return collider0 - position;
    if (count1 >= count2) return collider1 - position;
    return count2 > count0 ? collider2 - position : def;
}

bool Sensor::ObstacleSense(const std::vector<sf::Vector2u>& area) const {
    for (auto& cell : area) {
        for (const auto& ptr : Scene::getInstance()->obstacleGrid[cell.x][cell.y]) {
            if (const sf::Vector2f pos = ptr->position; (collider0 - pos).lengthSquared() < SENSE_RADIUS_SQR) {
                return true;
            }
        }
    }
    return false;
}

std::shared_ptr<Food> Sensor::SenseFood(const std::vector<sf::Vector2u>& area) const {
    if (position.x < 1 || position.x > 1270 || position.y < 1 || position.y > 710) return nullptr;

    for (auto& cell : area) {
        for (const auto& ptr : Scene::getInstance()->foodGrid[cell.x][cell.y]) {
            if (ptr->isHeld) {
                continue;
            }
            if (const sf::Vector2f pos = ptr->position;
                (collider0 - pos).lengthSquared() < SENSE_RADIUS_SQR
                || (collider1 - pos).lengthSquared() < SENSE_RADIUS_SQR
                || (collider2 - pos).lengthSquared() < SENSE_RADIUS_SQR) {
                return ptr;
                }
        }
    }
    return nullptr;
}




