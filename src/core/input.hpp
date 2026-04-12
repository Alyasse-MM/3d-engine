#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <cmath>
#include "state.hpp"

namespace al3d
{
    class InputManager {
    public:
        static void handleInput(sf::RenderWindow& window, EngineState& state);
    };
}