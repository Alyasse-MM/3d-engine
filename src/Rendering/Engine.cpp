#pragma once
#include "Engine.hpp"
#include "Core/input.hpp"

using namespace al3d;

Engine::Engine() :
    m_config(),
    m_state(),
    m_scene(),
    m_window(sf::VideoMode({ m_config.windowWidth, m_config.windowHeight }), "3D Engine", sf::Style::Default, sf::State::Windowed, sf::ContextSettings()),
    m_renderer(&m_window, &m_state, &m_config, &m_scene)
{
}

Engine::~Engine() {

}

void Engine::start() {
    m_scene.importMesh("..\\..\\..\\..\\tests\\references\\monkeyveryhigh\\monkey.obj");

    bool usePainter = false;

    while (m_window.isOpen()) {
        Core::InputManager::handleInput(m_window, m_state);
        m_renderer.renderZBuffer();
        m_window.display();
    }
}

void Engine::stop() {

}