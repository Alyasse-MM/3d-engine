#pragma once
#include "softwareRenderer.hpp"

class PainterRenderer : public SoftwareRenderer {
public:
    inline void paintersAlgorithm(std::vector<RenderFace>& drawList);
    void render(sf::RenderWindow& window, EngineState& state, Scene& scene);
};