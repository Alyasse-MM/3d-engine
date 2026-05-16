#pragma once
#include "EngineConfig.hpp"
#include "CameraState.hpp"
#include <Rendering/softwareRenderer.hpp>

class Engine {
public:
	al3d::Rendering::EngineConfig m_config;
	al3d::Rendering::CameraState m_state;
	al3d::Rendering::Scene m_scene;
	sf::RenderWindow m_window;
	al3d::Rendering::SoftwareRenderer m_renderer;

	Engine();
	~Engine();
	void start();
	void stop();
};