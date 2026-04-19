#include "ZBufferRenderer.hpp"
#include "Maths/Maths.hpp"
#include "Rendering/Clipping.hpp"
#include <algorithm>
#include <omp.h>

namespace al3d
{
    using namespace Maths;

    void ZBufferRenderer::workerLoop(unsigned threadId, unsigned nbThreads) {
        int sliceHeight = m_engineState.windowHeight / nbThreads;
        int threadStartY = threadId * sliceHeight;
        int threadEndY = (threadId == nbThreads - 1) ? (m_engineState.windowHeight - 1) : (threadStartY + sliceHeight - 1);

        {
            std::unique_lock<std::mutex> lock(m_printMtx);
            std::cout << "thread " << threadId << " started with " << threadStartY << " ; " << threadEndY << std::endl;
        }

        unsigned currentFrameId = 0;
        while (true) {
            {
                std::unique_lock<std::mutex> lock(m_syncMtx);
                m_cvStart.wait(lock, [this, currentFrameId] {
                    return (m_currentFrameId > currentFrameId) || m_stopThreads;
                    });
                if (m_stopThreads) return;
                currentFrameId = m_currentFrameId;
            }
            for (const RenderFace face : m_drawList) {
                if (face.points.size() != 3)
                    continue;
                float faceMinY = std::min({ face.points[0].y, face.points[1].y, face.points[2].y });
                float faceMaxY = std::max({ face.points[0].y, face.points[1].y, face.points[2].y });

                int faceStartY = std::max(threadStartY, (int)std::floor(faceMinY));
                int faceEndY = std::min(threadEndY, (int)std::ceil(faceMaxY));

                if (faceStartY > faceEndY) continue;

                float faceMinX = std::min({ face.points[0].x, face.points[1].x, face.points[2].x });
                float faceMaxX = std::max({ face.points[0].x, face.points[1].x, face.points[2].x });
                int faceStartX = std::max(0, (int)std::floor(faceMinX));
                int faceEndX = std::min((int)m_engineState.windowWidth - 1, (int)std::ceil(faceMaxX));

                drawTriangle(face, faceStartX, faceEndX, faceStartY, faceEndY);
            }

            {
                std::lock_guard<std::mutex> lock(m_syncMtx);
                m_nActiveThreads--;
                if (m_nActiveThreads == 0) {
                    m_cvDone.notify_one();
                }
            }
        }
    }

    void ZBufferRenderer::render() {
        m_drawList.clear();

        auto worldRotation = Matrix3<float>::getRotationX(toRadians(m_engineState.modelAngleX)) * Matrix3<float>::getRotationY(toRadians(m_engineState.modelAngleY));
        auto camRotation = Matrix3<float>::getRotationY(toRadians(-m_engineState.cameraYaw));

        std::vector<Vector3<float>> viewSpaceVertices;
        for (const auto& v : m_scene->m_vertices) {
            viewSpaceVertices.push_back(worldToView(v, worldRotation, camRotation, m_engineState.cameraPosition));
        }

        std::vector<Face>* m_faces{ prepareFacesToDraw(viewSpaceVertices) };

        clearBuffers();

        {
            std::lock_guard<std::mutex> lock(m_syncMtx);
            m_nActiveThreads = m_workers.size();
            m_currentFrameId++;
        }
        {
            std::unique_lock<std::mutex> lock(m_syncMtx);
            m_cvStart.notify_all();
        }

        {
            std::unique_lock<std::mutex> lock(m_syncMtx);
            m_cvDone.wait(lock, [this] { return m_nActiveThreads == 0; });
        }

        m_renderTexture.update(reinterpret_cast<const uint8_t*>(m_colorBuffer.data()));
        m_window.clear(sf::Color::Black);
        m_window.draw(m_renderSprite);
    }

    std::vector<Face>* ZBufferRenderer::prepareFacesToDraw(std::vector<Vector3<float>>& viewSpaceVertices) {
        std::vector<Face>* facesToDraw = new std::vector<Face>();

        for (const Face& face : m_scene->m_faces) {
            for (unsigned i = 1; i < face.indices.size() - 1; ++i) {
                Face tri = face;
                tri.indices = { face.indices[0], face.indices[i], face.indices[i + 1] };
                facesToDraw->push_back(tri);
            }
        }

        float focalLength = Maths::calculateFocalLength(m_engineState.fov, m_engineState.windowWidth);
        float halfWidth = m_engineState.windowWidth / 2.0f;
        float halfHeight = m_engineState.windowHeight / 2.0f;

        for (const auto& face : *facesToDraw) {
            std::vector<Vector3<float>> faceVerts;
            for (unsigned i : face.indices) faceVerts.push_back(viewSpaceVertices[i]);

            if (backfaceCulling(faceVerts)) continue;

            for (std::vector<Vector3<float>> clipped : Rendering::clipPolygon(faceVerts, m_engineState.nearClipPlane))
            {
                if (clipped.size() != 3) continue;

                std::vector<sf::Vector2f> screenPoints;
                std::vector<float> zCoords;
                float zSum = 0;
                for (const auto& vertex : clipped) {
                    screenPoints.push_back(perspectiveProjection(vertex, focalLength, halfWidth, halfHeight));
                    zCoords.push_back(vertex.z),
                        zSum += vertex.z;
                }

                m_drawList.push_back(RenderFace{
                    screenPoints,
                    zCoords,
                    face.color,
                    zSum / (float)clipped.size()
                    });
            }
        }

        return facesToDraw;
    }

    bool ZBufferRenderer::putPixel(int x, int y, float z, sf::Color color) {
        if (x < 0 || x >= m_engineState.windowWidth || y < 0 || y >= m_engineState.windowHeight) {
            return false;
        }
        int i = y * m_engineState.windowWidth + x;

        bool isNewZ = z < depthBuffers[i];
        if (isNewZ) {
            depthBuffers[i] = z;
            m_colorBuffer[i] = colorToUint32(color);
        }
        return isNewZ;
    }

    void ZBufferRenderer::drawTriangle(const RenderFace& triangle, int startX, int endX, int startY, int endY) {
        if (triangle.points.size() != 3) {
            std::cout << "not a triangle" << std::endl;
            return;
        }
        float triangleArea = perpProduct(triangle.points[0], triangle.points[1], triangle.points[2]);

        if (std::abs(triangleArea) < 0.000001f) return;

        for (int y = startY; y <= endY; ++y) {
            for (int x = startX; x <= endX; ++x) {
                sf::Vector2f pixel(x, y);

                float valueAB = perpProduct(triangle.points[0], triangle.points[1], pixel);
                float valueBC = perpProduct(triangle.points[1], triangle.points[2], pixel);
                float valueCA = perpProduct(triangle.points[2], triangle.points[0], pixel);

                if (insideTriangle(valueAB, valueBC, valueCA)) {
                    float weightC = valueAB / triangleArea;
                    float weightA = valueBC / triangleArea;
                    float weightB = valueCA / triangleArea;

                    float z = (weightA * triangle.zValues[0]) +
                        (weightB * triangle.zValues[1]) +
                        (weightC * triangle.zValues[2]);

                    putPixel(x, y, z, triangle.color);
                }
            }
        }
    }

    void ZBufferRenderer::clearBuffers() {
        std::fill(m_colorBuffer.begin(), m_colorBuffer.end(), 0xff949494);
        std::fill(depthBuffers.begin(), depthBuffers.end(), 10000.0f);
    }

}