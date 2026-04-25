#include <SFML/Graphics.hpp>
#include "Core/state.hpp"
#include "Rendering/scene.hpp"
#include "Rendering/painterRenderer.hpp"
#include "Maths/Maths.hpp"
#include "Core/input.hpp"
#include <Rendering/zbufferRenderer.hpp>
#include "Core/Mesh.hpp"

using namespace al3d;

int main() {
    EngineState state;
    Rendering::Scene* scene = new Rendering::Scene();
    scene->importMesh("D:\\Fichiers\\Projets_persos\\Programming_Projects\\GitHub\\al_3d\\resources\\references\\Skull\\Skull.obj");
    std::unique_ptr<Rendering::SoftwareRenderer> renderer;

    sf::ContextSettings settings;
    settings.antiAliasingLevel = state.antiAliasingLevel;

    sf::RenderWindow window(sf::VideoMode({ state.windowWidth, state.windowHeight }), "3D Engine", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    bool usePainter = false;

    if (usePainter) {
        renderer = std::make_unique<Rendering::PainterRenderer>(window, state);
    }
    else {
        renderer = std::make_unique<Rendering::ZBufferRenderer>(window, state);
    }

    while (window.isOpen()) {

        InputManager::handleInput(window, state);
        if (renderer)
            renderer->setScene(scene);
        renderer->render();
        window.display();
    }

    return 0;
}