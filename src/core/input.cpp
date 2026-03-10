#include "input.h"
#include "maths/Utils.h"

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
    float rotStep = 2.0f;
    float moveStep = 0.1f;

    if (sf::Keyboard::isKeyPressed(Key::Left)) state.model_angle_y += rotStep;
    if (sf::Keyboard::isKeyPressed(Key::Right)) state.model_angle_y -= rotStep;
    if (sf::Keyboard::isKeyPressed(Key::Up)) state.model_angle_x += rotStep;
    if (sf::Keyboard::isKeyPressed(Key::Down)) state.model_angle_x -= rotStep;

    if (sf::Keyboard::isKeyPressed(Key::A)) state.camera_yaw -= rotStep;
    if (sf::Keyboard::isKeyPressed(Key::E)) state.camera_yaw += rotStep;

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