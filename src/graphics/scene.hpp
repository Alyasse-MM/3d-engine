#pragma once
#include <SFML/Graphics.hpp>
#include "maths/Vector3.hpp"

struct Face {
    std::vector<int> indices;
    sf::Color color;
};

class Scene {
public:
    std::vector<Maths::Vector3<float>> vertices;
    std::vector<Face> faces;

    Scene();
};