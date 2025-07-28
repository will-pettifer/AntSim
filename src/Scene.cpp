//
// Created by billy on 18/07/2025.
//

#include "Scene.h"
#include "Helpers.h"
#include "Timer.h"

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


    {
        //Timer timer;
//#pragma omp parallel for num_threads(6)
        for (const auto& ptr : ants) {
            ptr->update(delta);
        }
    }


    {
        //Timer timer;
#pragma omp parallel for num_threads(6)
        for (const auto& ptr : foodMarkers) {
            ptr->update(delta);
        }
#pragma omp parallel for num_threads(6)
        for (const auto& ptr : homeMarkers) {
            ptr->update(delta);
        }
    }

    unloadMarkers();
    unloadFoods();
    unloadAnts();
}

void Scene::loadAnt(sf::Vector2f position) {
    ants.push_back(make_shared<Ant>(position));
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
    obstacleGrid[posi.x][posi.y].push_back(make_shared<Marker>(pos, Marker::Home));
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

    #pragma omp critical
    {
        int gridId = foodMarkerGrid[posi.x][posi.y].size();
        int id = foodMarkers.size();

        const auto ptr = make_shared<Marker>(pos, Marker::Food, id, gridId);
        foodMarkerGrid[posi.x][posi.y].push_back(ptr);
        foodMarkers.push_back(ptr);
    }
}
void Scene::loadHomeMarker(sf::Vector2f pos) {
    if (pos.x < 0 || pos.x > 1280 || pos.y < 0 || pos.y > 720) return;
    const auto posi = static_cast<sf::Vector2u>(pos / 10.f);

    #pragma omp critical
    {
        int gridId = homeMarkerGrid[posi.x][posi.y].size();
        int id = homeMarkers.size();

        const auto ptr = make_shared<Marker>(pos, Marker::Home, id, gridId);
        homeMarkerGrid[posi.x][posi.y].push_back(ptr);
        homeMarkers.push_back(ptr);
    }
}
void Scene::unloadMarkers() {
    for (const auto& marker : markersToRemove) {
        const auto pos = static_cast<sf::Vector2u>(marker->position / 10.f);
        const auto id = marker->id;
        const auto gridId = marker->gridId;

        if (marker->type == Marker::Food) {
            foodMarkers[id].swap(foodMarkers.back());
            foodMarkers[id]->id = id;
            foodMarkers.pop_back();

            auto& cell = foodMarkerGrid[pos.x][pos.y];
            cell[gridId].swap(cell.back());
            cell[gridId]->gridId = gridId;
            cell.pop_back();
        } else if (marker->type == Marker::Home) {
            homeMarkers[id].swap(homeMarkers.back());
            homeMarkers[id]->id = id;
            homeMarkers.pop_back();

            auto& cell = homeMarkerGrid[pos.x][pos.y];
            cell[gridId].swap(cell.back());
            cell[gridId]->gridId = gridId;
            cell.pop_back();
        }
    }

    markersToRemove.clear();
}

void Scene::addCell(const sf::Vector2u cell) {
    cellsToDisplay.push_back(cell);
}

void Scene::addCircle(const sf::Vector2f pos) {
    circlesToDisplay.push_back(pos);
}
