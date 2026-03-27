#pragma once
#include "softwareRenderer.hpp"

class PainterRenderer : public SoftwareRenderer {
public:
    PainterRenderer(sf::RenderWindow& w, EngineState& e) : SoftwareRenderer(w, e) {}
    inline void paintersAlgorithm(std::vector<RenderFace>& drawList);
    void render();
};