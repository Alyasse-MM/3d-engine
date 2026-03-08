#include <SFML/Graphics.hpp>
#include "core/state.h"
#include "graphics/scene.h"
#include "graphics/renderer.h"
#include "maths/Math.h"
#include "core/input.h"

int main() {
    // Setup Window
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "3D Engine", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    EngineState state;
    Scene scene;
    Renderer renderer;

    // Main Loop
    while (window.isOpen()) {
        
        InputManager::handleInput(window, state);

        window.clear(sf::Color::White);
        renderer.render(window, state, scene);
        window.display();
    }

    return 0;
}