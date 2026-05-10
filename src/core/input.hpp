#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <cmath>
#include "Rendering/CameraState.hpp"

namespace al3d
{
    namespace Core {
        class InputManager {
        public:
            static void handleInput(sf::RenderWindow& window, CameraState& state);
        };
    }
}