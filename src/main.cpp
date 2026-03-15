#include <SFML/Graphics.hpp>
#include "core/state.h"
#include "graphics/scene.h"
#include "graphics/renderer.h"
#include "maths/Math.h"
#include "core/input.h"

int main() {
    EngineState state;
    Scene scene;
    Renderer renderer;

    sf::ContextSettings settings;
    settings.antiAliasingLevel = state.antialiasinglevel;

    sf::RenderWindow window(sf::VideoMode({state.windowWidth, state.windowHeight}), "3D Engine", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        
        InputManager::handleInput(window, state);

        window.clear(sf::Color::White);
        renderer.render(window, state, scene);
        window.display();
    }

    return 0;
}