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

    struct ColliderBounds {
        float minX, maxX, minY, maxY;
        explicit ColliderBounds(const sf::Vector2f& collider) :
            minX(collider.x - SENSE_RADIUS),
            maxX(collider.x + SENSE_RADIUS),
            minY(collider.y - SENSE_RADIUS),
            maxY(collider.y + SENSE_RADIUS) {}
    };

    const ColliderBounds bounds0(collider0);
    const ColliderBounds bounds1(collider1);
    const ColliderBounds bounds2(collider2);

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            const sf::Vector2u celli = {coord.x + i, coord.y + j};

            const auto cell = static_cast<sf::Vector2f>(celli) * 10.f;
            float cellX = cell.x;
            float cellY = cell.y;
            float cellMaxX = cellX + 10.f;
            float cellMaxY = cellY + 10.f;

            if ((bounds0.maxX > cellX && bounds0.minX < cellMaxX
                 && bounds0.maxY > cellY && bounds0.minY < cellMaxY)
                || (bounds1.maxX > cellX && bounds1.minX < cellMaxX
                 && bounds1.maxY > cellY && bounds1.minY < cellMaxY)
                || (bounds2.maxX > cellX && bounds2.minX < cellMaxX
                 && bounds2.maxY > cellY && bounds2.minY < cellMaxY)) {
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
            if (ptr == nullptr) { continue; }
            if (const sf::Vector2f pos = ptr->position; (collider0 - pos).lengthSquared() < SENSE_RADIUS_SQR) {
                count0++;
            }
            else if ((collider1 - pos).lengthSquared() < SENSE_RADIUS_SQR) { count1++; }
            else if ((collider2 - pos).lengthSquared() < SENSE_RADIUS_SQR) { count2++; }
        }
    }

    //if (count0 > 100) return def;

    if (count0 >= count1 && count0 >= count2) return collider0 - position;
    if (count1 >= count0 && count1 >= count2) return collider1 - position;
    return collider2 - position;
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




