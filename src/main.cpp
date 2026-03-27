#include <SFML/Graphics.hpp>
#include "core/state.hpp"
#include "graphics/scene.hpp"
#include "graphics/softwareRenderer.hpp"
#include "graphics/painterRenderer.hpp"
#include "maths/Math.hpp"
#include "core/input.hpp"

int main() {
    EngineState state;
    Scene scene;
    std::unique_ptr<SoftwareRenderer> renderer;

    sf::ContextSettings settings;
    settings.antiAliasingLevel = state.antialiasinglevel;

    sf::RenderWindow window(sf::VideoMode({state.windowWidth, state.windowHeight}), "3D Engine", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    bool use_Painter = true;

    if (use_Painter) {
        renderer = std::make_unique<PainterRenderer>();
    }

    while (window.isOpen()) {
        
        InputManager::handleInput(window, state);

        window.clear(sf::Color::White);
        renderer->render(window, state, scene);
        window.display();
    }

    return 0;
}