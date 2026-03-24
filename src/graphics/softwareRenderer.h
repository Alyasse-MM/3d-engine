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

class SoftwareRenderer {
public:
    virtual ~SoftwareRenderer() = default;
    virtual void render(sf::RenderWindow& window, EngineState& state, Scene& scene) = 0;
    inline bool backfaceCulling(const std::vector<Vector3<float>>& faceVerts);
};