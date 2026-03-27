#pragma once
#include "softwareRenderer.hpp"

class ZBufferRenderer : public SoftwareRenderer {
private:
    std::vector<uint32_t> color_buffer;
    std::vector<float> z_buffer;

public:
    ZBufferRenderer(sf::RenderWindow& w, EngineState& e) : SoftwareRenderer(w, e),
        z_buffer(e.windowWidth* e.windowHeight, 1.0f),
        color_buffer(e.windowWidth* e.windowHeight, 0x000000FF) {}
    void render();
    void clear();
};