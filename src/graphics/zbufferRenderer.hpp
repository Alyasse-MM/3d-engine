#pragma once
#include "softwareRenderer.hpp"
#include <iostream>
#include <thread>
#include <mutex>

class ZBufferRenderer : public SoftwareRenderer {
private:
    std::vector<uint32_t> color_buffer;
    std::vector<float> z_buffer;
    sf::Texture renderTexture;
    sf::Sprite renderSprite;

    std::vector<RenderFace> drawList;

    std::vector<std::thread> workers;
    std::mutex mtx;
    std::mutex printMtx;
    std::condition_variable cv_start;
    std::condition_variable cv_done;

    int active_threads = 0;
    bool stop_threads = false;
    bool frame_ready = false;

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
        unsigned n = std::thread::hardware_concurrency();
        n = std::max(unsigned(n*0.75), unsigned(2));
        for (unsigned i = 0; i < n; ++i) {
            workers.push_back(std::thread(&ZBufferRenderer::workerLoop, this, i, n));
        }
    }

    ~ZBufferRenderer() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            stop_threads = true;
        }
        cv_start.notify_all();

        for (auto& t : workers) {
            if (t.joinable()) t.join();
        }
    }

    void workerLoop(unsigned threadID, unsigned numThreads);

    void render();
    bool putPixel(int x, int y, float z, sf::Color color);
    void drawTriangle(const RenderFace& f, int startX, int endX, int startY, int endY);
    inline uint32_t colorToUint32(sf::Color color) {
        return (uint32_t)(color.r | (color.g << 8) | (color.b << 16) | (color.a << 24));
    }
    void clear();
};