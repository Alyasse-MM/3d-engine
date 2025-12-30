#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Maths.h"

struct Face {
    std::vector<int> indices;
    sf::Color color;
};

class Scene {
public:
    std::vector<Vec3> vertices;
    std::vector<Face> faces;

    Scene() {
        vertices = {
            {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
            {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1},
            {4, -1, 4}, {6, -1, 4}, {6, 1, 4}, {4, 1, 4},
            {4, -1, 6}, {6, -1, 6}, {6, 1, 6}, {4, 1, 6}
        };

        faces = {
            {{0, 3, 2, 1}, sf::Color::Red},
            {{4, 5, 6, 7}, sf::Color::Cyan},
            {{0, 4, 7, 3}, sf::Color::Green},
            {{1, 2, 6, 5}, sf::Color::Blue},
            {{3, 7, 6, 2}, sf::Color::Yellow},
            {{0, 1, 5, 4}, sf::Color::Magenta},
            // Second Cube
            {{8, 11, 10, 9}, sf::Color::Red},
            {{12, 13, 14, 15}, sf::Color::Cyan},
            {{8, 12, 15, 11}, sf::Color::Green},
            {{9, 10, 14, 13}, sf::Color::Blue},
            {{11, 15, 14, 10}, sf::Color::Yellow},
            {{8, 9, 13, 12}, sf::Color::Magenta}
        };
    }
};