#pragma once
#include "softwareRenderer.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include "../maths/Vector3.hpp"

class ZBufferRenderer : public SoftwareRenderer {
private:
    std::vector<uint32_t> m_colorBuffer;
    std::vector<float> depthBuffers;
    sf::Texture m_renderTexture;
    sf::Sprite m_renderSprite;

    std::vector<RenderFace> m_drawList;

    std::vector<std::thread> m_workers;
    std::mutex m_syncMtx;
    std::mutex m_printMtx;
    std::condition_variable m_cvStart;
    std::condition_variable m_cvDone;

    int m_nActiveThreads = 0;
    bool m_stopThreads = false;
    unsigned m_currentFrameId = 0;

    std::vector<Face>* prepareFacesToDraw(std::vector<Maths::Vector3<float>>& viewSpaceVertices);
    inline uint32_t colorToUint32(sf::Color color) {
        return (uint32_t)(color.r | (color.g << 8) | (color.b << 16) | (color.a << 24));
    }
    void workerLoop(unsigned threadID, unsigned nbThreads);
    bool putPixel(int x, int y, float z, sf::Color color);
    void drawTriangle(const RenderFace& f, int startX, int endX, int startY, int endY);
    void clearBuffers();

public:
    ZBufferRenderer(sf::RenderWindow& w, EngineState& e) : SoftwareRenderer(w, e),

        depthBuffers(e.windowWidth* e.windowHeight, 10000.0f),
        m_colorBuffer(e.windowWidth* e.windowHeight, 0xff949494),
        m_renderSprite(m_renderTexture) {
        m_renderSprite.setColor(sf::Color::White);
        if (!m_renderTexture.resize({ (unsigned int)e.windowWidth, (unsigned int)e.windowHeight })) {
            std::cerr << "Failed to initialize render texture!" << std::endl;
        }
        m_renderSprite.setTexture(m_renderTexture, true);
        unsigned n = std::thread::hardware_concurrency();
        n = std::max(unsigned(n*0.75), unsigned(2));
        for (unsigned i = 0; i < n; ++i) {
            m_workers.push_back(std::thread(&ZBufferRenderer::workerLoop, this, i, n));
        }
    }

    ~ZBufferRenderer() {
        {
            std::lock_guard<std::mutex> lock(m_syncMtx);
            m_stopThreads = true;
        }
        m_cvStart.notify_all();

        for (auto& t : m_workers) {
            if (t.joinable()) t.join();
        }
    }

    void render();
};