//
// Created by billy on 18/07/2025.
//
#pragma once

#include "Ant.h"
#include <vector>
#include <list>
#include <memory>

#include "Marker.h"
#include "Food.h"

using namespace std;

class Scene {
protected:
    Scene();
    static Scene* instance;

public:
    Scene(const Scene& obj) = delete;

    static Scene* getInstance();

    sf::Vector2f mousePosition;

    void update(float delta);

    int score = 0;
    int vertexCount = 0;


    vector<shared_ptr<Ant>> ants;
    vector<shared_ptr<Ant>> antsToRemove;
    void loadAnt(sf::Vector2f position);
    void unloadAnts();

    vector<shared_ptr<Food>> foodGrid[128][72];
    vector<shared_ptr<Food>> heldFoods;
    vector<shared_ptr<Food>> foodsToRemove;
    void loadFood(sf::Vector2f pos);
    void unloadFoods();

    vector<shared_ptr<Marker>> obstacleGrid[128][72];
    vector<shared_ptr<Marker>> obstaclesToRemove;
    void loadObstacle(sf::Vector2f pos);
    void unloadObstacles();

    vector<shared_ptr<Marker>> foodMarkers;
    vector<shared_ptr<Marker>> foodMarkerGrid[128][72];
    vector<shared_ptr<Marker>> homeMarkers;
    vector<shared_ptr<Marker>> homeMarkerGrid[128][72];
    vector<shared_ptr<Marker>> markersToRemove;
    void loadFoodMarker(sf::Vector2f pos);
    void loadHomeMarker(sf::Vector2f pos);
    void unloadMarkers();

    vector<sf::Vector2u> cellsToDisplay;
    void addCell(sf::Vector2u cell);
    vector<sf::Vector2f> circlesToDisplay;
    void addCircle(sf::Vector2f pos);
};
