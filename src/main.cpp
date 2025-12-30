#include <SFML/Graphics.hpp>
#include "State.h"
#include "Scene.h"
#include "Renderer.h"
#include "Maths.h"
#include "Inputs.h"

int main() {
    // Setup Window
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    
    sf::RenderWindow window(sf::VideoMode({800, 600}), "3D Engine (SFML 3)", sf::Style::Default, sf::State::Windowed, settings);
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