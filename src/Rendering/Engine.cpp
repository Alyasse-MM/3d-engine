#pragma once
#include "Engine.hpp"
#include "Core/input.hpp"
#include "Core/MeshInstance.hpp"
using namespace al3d::Maths;

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
    m_scene.loadMeshInstance(
        "..\\..\\..\\..\\tests\\references\\Skull\\Skull.obj",
        { 0.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f },
        0.1f);

    bool usePainter = false;

    while (m_window.isOpen()) {
        Core::InputManager::handleInput(m_window, m_state);
        m_renderer.renderZBuffer();
        m_window.display();
    }
}

void Engine::stop() {

}