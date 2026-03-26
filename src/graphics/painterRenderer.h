#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "core/state.h"
#include "scene.h"
#include "softwareRenderer.h"

class PainterRenderer : public SoftwareRenderer {
public:
    inline void paintersAlgorithm(std::vector<RenderFace>& drawList);
    void render(sf::RenderWindow& window, EngineState& state, Scene& scene);
};