#include <iostream>
#include <SFML/Graphics.hpp>

#include "Helpers.h"
#include "Random.h"
#include "Scene.h"
#include "Sensor.h"


int main()
{
    constexpr sf::Vector2u windowSize = {1280u, 720u};
    constexpr sf::Vector2f windowCentre = {windowSize.x / 2.f, windowSize.y / 2.f};
    auto window = sf::RenderWindow(sf::VideoMode(windowSize), "CMake SFML Project");
    window.setFramerateLimit(144);
    window.setPosition({200, 100});



    // --- Pre Loop ---

    Scene* scene = Scene::getInstance();
    sf::Clock clock;
    float time = 0;

    int fpsCounter = 0;
    float fpsTimer = 0;
    vector<int> fps;
    float fpsAverage = 0;

    constexpr float LIFETIME = Marker::LIFETIME;
    constexpr float TRANSLUCENCY = 90.f;

    auto vertices = sf::VertexArray(sf::PrimitiveType::Points, 5000);


    for (int i = 0; i < 400; ++i) {
        scene->loadAnt(windowCentre);
    }

    for (int i = 0; i < 3; ++i) {
        sf::Vector2f position = Random::OnUnitCircle() * Random::Float(150.f, 300.f);
        position += windowCentre;
        for (int j = 0; j < 150; ++j) {
            scene->loadFood(position + Random::OnUnitCircle() * Random::Float(0.f, 20.f));
        }
    }


    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            {
                cout << "fps average: " << fpsAverage << endl;
                window.close();
            }
        }

        const float delta = clock.getElapsedTime().asSeconds() - time;
        time = clock.getElapsedTime().asSeconds();



        // --- Game Loop ---

        fpsCounter++;
        fpsTimer += delta;
        if (fpsTimer > 1.f) {
            fpsTimer -= 1.f;
            fps.push_back(fpsCounter);
            cout << "fps: " << fpsCounter << endl;
            fpsCounter = 0;
            cout << "score: " << scene->score << endl;
        }
        if (fps.size() == 180) {
            for (const int & fp : fps) {
                fpsAverage += fp;
            }
            fpsAverage /= fps.size();
            cout << "fps average: " << fpsAverage << endl;
            fps.clear();
        }


        scene->update(delta);

        scene->mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            scene->loadObstacle(scene->mousePosition);
        }


        // --- Draw Loop ---

        window.clear();

        vertices.resize(scene->vertexCount);

        int index = 0;
        for (auto & i : scene->homeMarkerGrid) {
            for (auto & j : i) {
                for (const auto& ptr : j) {
                    vertices[index].position = ptr->position;
                    vertices[index].color = sf::Color(50, 100, 255, TRANSLUCENCY * (ptr->lifetime / LIFETIME));
                    index++;
                }
            }
        }
        for (auto & i : scene->foodMarkerGrid) {
            for (auto & j : i) {
                for (const auto& ptr : j) {
                    vertices[index].position = ptr->position;
                    vertices[index].color = sf::Color(20, 255, 20, TRANSLUCENCY * (ptr->lifetime / LIFETIME));
                    index++;
                }
            }
        }
        for (const auto& ptr : scene->ants) {
            vertices[index].position = ptr->position;
            vertices[index].color = sf::Color(255, 255, 255);
            index++;
        }
        for (auto & i : scene->foodGrid) {
            for (auto & j : i) {
                for (const auto& ptr : j) {
                    vertices[index].position = ptr->position;
                    vertices[index].color = sf::Color(255, 255, 0);
                    index++;
                }
            }
        }
        for (auto & i : scene->obstacleGrid) {
            for (auto & j : i) {
                for (const auto& ptr : j) {
                    vertices[index].position = ptr->position;
                    vertices[index].color = sf::Color(255, 0, 0);
                    index++;
                }
            }
        }

        window.draw(vertices);

        window.display();
    }
}
