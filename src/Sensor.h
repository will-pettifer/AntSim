//
// Created by billy on 20/07/2025.
//

#ifndef SENSOR_H
#define SENSOR_H
#include <vector>
#include <memory>

#include "Food.h"
#include "Marker.h"
#include "SFML/System/Vector2.hpp"


class Sensor {
private:


    sf::Vector2f position;
    sf::Vector2f direction;

    static constexpr float SENSE_DISTANCE = 7.f;
    static constexpr float SENSE_ANGLE = sf::priv::pi / 3.7f;
    static constexpr float SENSE_RADIUS = 6.f;
    static constexpr float SENSE_RADIUS_SQR = SENSE_RADIUS * SENSE_RADIUS;

    sf::Vector2f collider0;
    sf::Vector2f collider1;
    sf::Vector2f collider2;
    float count0;
    float count1;
    float count2;

public:
    Sensor(sf::Vector2f position, sf::Vector2f direction);

    [[nodiscard]] std::vector<sf::Vector2u> GetSenseArea() const;
    sf::Vector2f MultiSense(const std::vector<std::shared_ptr<Marker>> markers[128][72], const std::vector<sf::Vector2u>& area, sf::Vector2f def);
    [[nodiscard]] bool ObstacleSense(const std::vector<sf::Vector2u>& area) const;
    [[nodiscard]] std::shared_ptr<Food> SenseFood(const std::vector<sf::Vector2u>& area) const;
};



#endif //SENSOR_H
