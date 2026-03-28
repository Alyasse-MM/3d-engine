#pragma once
#include "softwareRenderer.hpp"
#include <iostream>

class ZBufferRenderer : public SoftwareRenderer {
private:
    std::vector<uint32_t> color_buffer;
    std::vector<float> z_buffer;
    sf::Texture renderTexture;
    sf::Sprite renderSprite;

public:
    ZBufferRenderer(sf::RenderWindow& w, EngineState& e) : SoftwareRenderer(w, e),
        z_buffer(e.windowWidth* e.windowHeight, 1.0f),
        color_buffer(e.windowWidth* e.windowHeight, 0x000000FF),
        renderSprite(renderTexture) {
        renderSprite.setColor(sf::Color::White);
        if (!renderTexture.resize({ (unsigned int)e.windowWidth, (unsigned int)e.windowHeight })) {
            std::cerr << "Failed to initialize render texture!" << std::endl;
        }
        renderSprite.setTexture(renderTexture, true);
    }
    void render();
    bool putPixel(int x, int y, float z, sf::Color color);
    void drawTriangle(const RenderFace& f);
    inline uint32_t colorToUint32(sf::Color color) {
        return (uint32_t)(color.r | (color.g << 8) | (color.b << 16) | (color.a << 24));
    }
    void clear();
};