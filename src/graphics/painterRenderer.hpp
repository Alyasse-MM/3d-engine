#pragma once
#include "softwareRenderer.hpp"

class PainterRenderer : public SoftwareRenderer {
private:
    inline void paintersAlgorithm(std::vector<RenderFace>& drawList);
public:
    PainterRenderer(sf::RenderWindow& w, EngineState& e) : SoftwareRenderer(w, e) {}
    void render();
};