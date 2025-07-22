//
// Created by billy on 18/07/2025.
//

#include "Scene.h"

#include <iostream>
#include <vector>
#include <memory>


using namespace std;


Scene::Scene() {
}

Scene* Scene::instance = nullptr;
Scene* Scene::getInstance() {
    if (instance == nullptr) {
        instance = new Scene();
    }
    return instance;
}

void Scene::update(const float delta) {
    for (const auto& ptr : ants) {
        ptr->update(delta);
    }
    for (int i = 0; i < 128; i++) {
        for (int j = 0; j < 72; j++) {
            for (const auto& ptr : foodGrid[i][j]) {
                ptr->update(delta);
            }
            for (const auto& ptr : homeMarkerGrid[i][j]) {
                ptr->update(delta);
            }
            for (const auto& ptr : foodMarkerGrid[i][j]) {
                ptr->update(delta);
            }
        }
    }


    unloadMarkers();
    unloadFoods();
    unloadAnts();
}

void Scene::loadAnt(sf::Vector2f position) {
    ants.push_back(make_shared<Ant>(position));
    vertexCount++;
}
void Scene::unloadAnts() {
    antsToRemove.clear();
}

void Scene::loadFood(sf::Vector2f pos) {
    if (pos.x < 0 || pos.x > 1280 || pos.y < 0 || pos.y > 720) return;
    const auto posi = static_cast<sf::Vector2u>(pos / 10.f);
    foodGrid[posi.x][posi.y].push_back(make_shared<Food>(pos));
    vertexCount++;
}
void Scene::unloadFoods() {
    for (const auto& food : foodsToRemove) {
        for (auto it = heldFoods.begin(); it != heldFoods.end(); ++it) {
            if (*it == food) {
                heldFoods.erase(it);
                vertexCount--;
                break;
            }
        }
    }
    foodsToRemove.clear();
}

void Scene::loadObstacle(sf::Vector2f pos) {
    if (pos.x < 0 || pos.x > 1280 || pos.y < 0 || pos.y > 720) return;
    const auto posi = static_cast<sf::Vector2u>(pos / 10.f);
    obstacleGrid[posi.x][posi.y].push_back(make_shared<Marker>(pos));
    vertexCount++;
}

void Scene::unloadObstacles() {
    for (const auto& obstacle : obstaclesToRemove) {
        const auto pos = static_cast<sf::Vector2u>(obstacle->position / 10.f);
        auto& cell = obstacleGrid[pos.x][pos.y];

        for (auto it = cell.begin(); it != cell.end(); ++it) {
            if (*it == obstacle) {
                cell.erase(it);
                vertexCount--;
                break;
            }
        }
    }
    obstaclesToRemove.clear();
}


void Scene::loadFoodMarker(sf::Vector2f pos) {
    if (pos.x < 0 || pos.x > 1280 || pos.y < 0 || pos.y > 720) return;
    const auto posi = static_cast<sf::Vector2u>(pos / 10.f);
    foodMarkerGrid[posi.x][posi.y].push_back(make_shared<Marker>(pos));
    vertexCount++;
}
void Scene::loadHomeMarker(sf::Vector2f pos) {
    if (pos.x < 0 || pos.x > 1280 || pos.y < 0 || pos.y > 720) return;
    const auto posi = static_cast<sf::Vector2u>(pos / 10.f);
    homeMarkerGrid[posi.x][posi.y].push_back(make_shared<Marker>(pos));
    vertexCount++;
}
void Scene::unloadMarkers() {
    for (const auto& marker : markersToRemove) {
        const auto pos = static_cast<sf::Vector2u>(marker->position / 10.f);
        auto& cellf = foodMarkerGrid[pos.x][pos.y];

        for (auto it = cellf.begin(); it != cellf.end(); ++it) {
            if (*it == marker) {
                cellf.erase(it);
                vertexCount--;
                break;
            }
        }

        auto& cellh = homeMarkerGrid[pos.x][pos.y];
        for (auto it = cellh.begin(); it != cellh.end(); ++it) {
            if (*it == marker) {
                cellh.erase(it);
                vertexCount--;
                break;
            }
        }
    }
    markersToRemove.clear();
}

void Scene::addCell(const sf::Vector2u cell) {
    cellsToDisplay.push_back(cell);
}

void Scene::addCircle(sf::Vector2f pos) {
    circlesToDisplay.push_back(pos);
}
