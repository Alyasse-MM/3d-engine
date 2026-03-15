#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "core/state.h"
#include "scene.h"

struct RenderFace {
    std::vector<sf::Vector2f> points;
    sf::Color color;
    float avgZ;
};

class Renderer {
public:
    void render(sf::RenderWindow& window, EngineState& state, Scene& scene);
};