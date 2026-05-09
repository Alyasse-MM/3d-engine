#include <SFML/Graphics.hpp>
#include "Core/state.hpp"
#include "Rendering/scene.hpp"
#include "Maths/Maths.hpp"
#include "Core/input.hpp"
#include "Core/Mesh.hpp"

using namespace al3d;

int main() {
    EngineState state;
    Rendering::Scene scene{};
    scene.importMesh("D:\\Fichiers\\Projets_persos\\Programming_Projects\\GitHub\\al_3d\\resources\\references\\Skull\\Skull.obj");
    std::unique_ptr<Rendering::SoftwareRenderer> renderer;

    sf::ContextSettings settings;
    settings.antiAliasingLevel = state.antiAliasingLevel;

    sf::RenderWindow window(sf::VideoMode({ state.windowWidth, state.windowHeight }), "3D Engine", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    bool usePainter = true;

    renderer = std::make_unique<Rendering::SoftwareRenderer>(&window, &state, &scene);

    while (window.isOpen()) {

        InputManager::handleInput(window, state);
        renderer->renderZBuffer();
        window.display();
    }

    return 0;
}