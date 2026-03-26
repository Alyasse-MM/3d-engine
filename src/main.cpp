#include <SFML/Graphics.hpp>
#include "core/state.h"
#include "graphics/scene.h"
#include "graphics/softwareRenderer.h"
#include "maths/Math.h"
#include "core/input.h"

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
        renderer = std::make_unique<PainterRenderer>(width, height);
    }

    while (window.isOpen()) {
        
        InputManager::handleInput(window, state);

        window.clear(sf::Color::White);
        renderer->render(window, state, scene);
        window.display();
    }

    return 0;
}