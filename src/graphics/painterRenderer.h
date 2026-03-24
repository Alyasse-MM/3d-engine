#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "core/state.h"
#include "scene.h"
#include "softwareRenderer.h"

class PainterRenderer : public SoftwareRenderer {
public:
    void render(sf::RenderWindow& window, EngineState& state, Scene& scene);
};