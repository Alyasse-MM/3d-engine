#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <cmath>
#include "state.h"
#include "graphics/renderer.h"

class InputManager {
public:
    static void handleInput(sf::RenderWindow& window, EngineState& state);
};