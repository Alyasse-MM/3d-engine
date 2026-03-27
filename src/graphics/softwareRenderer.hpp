#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "core/state.hpp"
#include "scene.hpp"
#include "maths/Vector3.hpp"

struct RenderFace {
    std::vector<sf::Vector2f> points;
    sf::Color color;
    float avgZ;
};

class SoftwareRenderer {
protected:
    EngineState& enginestate;
    sf::RenderWindow& window;
    Scene* scene;
public:
    virtual ~SoftwareRenderer() = default;
    SoftwareRenderer(sf::RenderWindow& w, EngineState& e) : enginestate(e), window(w), scene(nullptr) {};
    virtual void render() = 0;
    void setScene(Scene* s) { scene = s; }
};