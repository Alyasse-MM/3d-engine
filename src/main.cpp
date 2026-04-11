#include <SFML/Graphics.hpp>
#include "core/state.hpp"
#include "graphics/scene.hpp"
#include "graphics/painterRenderer.hpp"
#include "maths/Math.hpp"
#include "core/input.hpp"
#include <graphics/zbufferRenderer.hpp>

int main() {
    EngineState state;
    Scene* scene = new Scene();
    std::unique_ptr<SoftwareRenderer> renderer;

    sf::ContextSettings settings;
    settings.antiAliasingLevel = state.antiAliasingLevel;

    sf::RenderWindow window(sf::VideoMode({state.windowWidth, state.windowHeight}), "3D Engine", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    bool usePainter = false;

    if (usePainter) {
        renderer = std::make_unique<PainterRenderer>(window, state);
    }else{
        renderer = std::make_unique<ZBufferRenderer>(window,state);
    }

    while (window.isOpen()) {
        
        InputManager::handleInput(window, state);
        if(renderer)
            renderer->setScene(scene);
            renderer->render();
        window.display();
    }

    return 0;
}