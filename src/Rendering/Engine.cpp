#pragma once
#include "Engine.hpp"
#include "Core/input.hpp"
#include "Core/MeshInstance.hpp"

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
    m_scene.loadMeshInstance("..\\..\\..\\..\\tests\\references\\monkeyveryhigh\\monkey.obj");
    m_scene.loadMeshInstance(
        "..\\..\\..\\..\\tests\\references\\monkeyveryhigh\\monkey.obj",
        {0.f, 0.f, 200.f},
        {0.f, 0.f, 0.f},
        2.f);

    bool usePainter = false;

    while (m_window.isOpen()) {
        Core::InputManager::handleInput(m_window, m_state);
        m_renderer.renderZBuffer();
        m_window.display();
    }
}

void Engine::stop() {

}