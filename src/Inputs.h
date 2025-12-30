#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <cmath>
#include "State.h"
#include "Maths.h"

class InputManager {
public:
    static void handleInput(sf::RenderWindow& window, EngineState& state) {
        
        // Window Events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape)
                    window.close();
            }
        }

        // Continuous Input
        using Key = sf::Keyboard::Key; 
        float rotStep = 2.0f;
        float moveStep = 0.1f;

        // Rotation
        if (sf::Keyboard::isKeyPressed(Key::Left)) state.angle_y += rotStep;
        if (sf::Keyboard::isKeyPressed(Key::Right)) state.angle_y -= rotStep;
        if (sf::Keyboard::isKeyPressed(Key::Up)) state.angle_x += rotStep;
        if (sf::Keyboard::isKeyPressed(Key::Down)) state.angle_x -= rotStep;

        if (sf::Keyboard::isKeyPressed(Key::A)) state.camera_yaw -= rotStep;
        if (sf::Keyboard::isKeyPressed(Key::E)) state.camera_yaw += rotStep;

        // Camera Movements
        float rad = Maths::toRadians(state.camera_yaw);
        float s = std::sin(rad);
        float c = std::cos(rad);

        if (sf::Keyboard::isKeyPressed(Key::W) || sf::Keyboard::isKeyPressed(Key::Z)) {
            state.camera_pos.x += s * moveStep;
            state.camera_pos.z += c * moveStep;
        }
        if (sf::Keyboard::isKeyPressed(Key::S)) {
            state.camera_pos.x -= s * moveStep;
            state.camera_pos.z -= c * moveStep;
        }
        if (sf::Keyboard::isKeyPressed(Key::Q)) {
            state.camera_pos.x -= c * moveStep;
            state.camera_pos.z += s * moveStep;
        }
        if (sf::Keyboard::isKeyPressed(Key::D)) {
            state.camera_pos.x += c * moveStep;
            state.camera_pos.z -= s * moveStep;
        }
    }
};