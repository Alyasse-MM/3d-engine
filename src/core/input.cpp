#include "input.hpp"
#include "Maths/Utils.hpp"

namespace al3d
{
    void InputManager::handleInput(sf::RenderWindow& window, EngineState& state) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape)
                    window.close();
            }
        }

        using Key = sf::Keyboard::Key;
        float rotationStep = 2.0f;
        float moveStep = 0.1f;

        if (sf::Keyboard::isKeyPressed(Key::Left)) state.modelAngleY += rotationStep;
        if (sf::Keyboard::isKeyPressed(Key::Right)) state.modelAngleY -= rotationStep;
        if (sf::Keyboard::isKeyPressed(Key::Up)) state.modelAngleX += rotationStep;
        if (sf::Keyboard::isKeyPressed(Key::Down)) state.modelAngleX -= rotationStep;

        if (sf::Keyboard::isKeyPressed(Key::A)) state.cameraYaw -= rotationStep;
        if (sf::Keyboard::isKeyPressed(Key::E)) state.cameraYaw += rotationStep;

        float rad = Maths::toRadians(state.cameraYaw);
        float sinValue = std::sin(rad);
        float cosValue = std::cos(rad);

        if (sf::Keyboard::isKeyPressed(Key::W) || sf::Keyboard::isKeyPressed(Key::Z)) {
            state.cameraPosition.x += sinValue * moveStep;
            state.cameraPosition.z += cosValue * moveStep;
        }
        if (sf::Keyboard::isKeyPressed(Key::S)) {
            state.cameraPosition.x -= sinValue * moveStep;
            state.cameraPosition.z -= cosValue * moveStep;
        }
        if (sf::Keyboard::isKeyPressed(Key::Q)) {
            state.cameraPosition.x -= cosValue * moveStep;
            state.cameraPosition.z += sinValue * moveStep;
        }
        if (sf::Keyboard::isKeyPressed(Key::D)) {
            state.cameraPosition.x += cosValue * moveStep;
            state.cameraPosition.z -= sinValue * moveStep;
        }
    }
}