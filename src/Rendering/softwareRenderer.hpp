#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "CameraState.hpp"
#include "scene.hpp"
#include "Maths/Vector3.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include "EngineConfig.hpp"

namespace al3d
{
    namespace Rendering
    {
        using Vector3f = Maths::Vector3<float>;

        struct RenderFace {
            std::vector<sf::Vector2f> points;
            std::vector<float> zValues;
            sf::Color color;
            float avgZ;
        };

        class SoftwareRenderer {
        private:
            EngineConfig* m_engineConfig;
            CameraState* m_engineState;
            sf::RenderWindow* m_window;
            Scene* m_scene;

            std::vector<RenderFace> m_drawList;
            std::vector<RenderFace> m_drawList_next;

            std::vector<Vector3f> viewSpaceVertices;
            std::vector<Vector3f> verticesNormals;

            std::vector<unsigned> m_facesIds;
            std::vector<unsigned> m_normalsIds;
            std::vector<sf::Color> m_facesColors;

            std::vector<uint32_t> m_colorBuffer;
            std::vector<float> m_depthBuffers;
            sf::Texture m_renderTexture;
            sf::Sprite m_renderSprite;

            std::vector<std::thread> m_workers;
            std::mutex m_syncMtx;
            std::mutex m_printMtx;
            std::condition_variable m_cvStart;
            std::condition_variable m_cvDone;

            int m_nActiveThreads = 0;
            bool m_stopThreads = false;
            unsigned m_currentFrameId = 0;

            static enum ThreadCapacity {
                min = 1,
                low = 25,
                medium = 50,
                high = 75,
                veryhigh = 90,
                max = 100
            };

            static enum LightDirection {
                zPositive = 1,
                zNegative = 2,
                xPositive = 3,
                xNegative = 4,
                yPositive = 5,
                yNegative = 6
            };

            static enum DepthManager {
                painter = 1,
                zbuffer = 2
            };

            void prepareFaces();
            inline uint32_t colorToUint32(sf::Color color) {
                return (uint32_t)(color.r | (color.g << 8) | (color.b << 16) | (color.a << 24));
            }

            Vector3f getLightNormal(LightDirection l);
            void workerLoop_zBuffer(unsigned threadID, unsigned nbThreads);
            bool putPixel(int x, int y, float z, sf::Color color);
            void drawTriangle(const RenderFace& f, int startX, int endX, int startY, int endY);
            void clearBuffers();
            inline void paintersAlgorithm(std::vector<RenderFace>& drawList);
        public:
            SoftwareRenderer(sf::RenderWindow* w, CameraState* e, EngineConfig* c, Scene* s) :
                m_depthBuffers(c->windowWidth* c->windowHeight, 10000.0f),
                m_colorBuffer(c->windowWidth* c->windowHeight, 0xff949494),
                m_renderSprite(m_renderTexture), m_window(w), m_engineState(e),
                m_engineConfig(c), m_scene(s), m_drawList(), m_drawList_next()
            {
                m_renderSprite.setColor(sf::Color::White);
                if (!m_renderTexture.resize({ (unsigned int)c->windowWidth, (unsigned int)c->windowHeight })) {
                    std::cerr << "Failed to initialize render texture!" << std::endl;
                }
                m_renderSprite.setTexture(m_renderTexture, true);
                unsigned n = std::thread::hardware_concurrency();
                n = std::max(unsigned(n * ThreadCapacity::high / 100), unsigned(2));
                for (unsigned i = 0; i < n; ++i) {
                    m_workers.push_back(std::thread(&SoftwareRenderer::workerLoop_zBuffer, this, i, n));
                };
            }

            ~SoftwareRenderer() {
                {
                    std::lock_guard<std::mutex> lock(m_syncMtx);
                    m_stopThreads = true;
                }
                m_cvStart.notify_all();

                for (auto& t : m_workers) {
                    if (t.joinable()) t.join();
                }
            }

            void renderZBuffer();
            void renderPainter();
        };
    }
}