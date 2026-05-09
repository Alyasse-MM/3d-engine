#pragma once
#include "softwareRenderer.hpp"
#include "Maths/Maths.hpp"
#include "Rendering/Clipping.hpp"
#include <algorithm>

namespace al3d
{
    namespace Rendering
    {
        using namespace Maths;
        using Face = Rendering::Face;

        void SoftwareRenderer::prepareFaces() {
            auto worldRotation = Matrix3<float>::getRotationX(toRadians(m_engineState->modelAngleX)) * Matrix3<float>::getRotationY(toRadians(m_engineState->modelAngleY));
            auto camRotation = Matrix3<float>::getRotationY(toRadians(-m_engineState->cameraYaw));

            unsigned refIdFaces = 0;
            unsigned refIdNormals = 0;

            viewSpaceVertices.clear();
            verticesNormals.clear();
            std::vector<Vector3f> vertices, normals;
            for (const auto& m : m_scene->m_meshes) {
                vertices = m->getVertices();
                normals = m->getNormals();
                for (int i = 0; i < vertices.size(); i++) {
                    viewSpaceVertices.push_back(worldToView(vertices[i], worldRotation, camRotation, m_engineState->cameraPosition));
                    verticesNormals.push_back(worldToViewNormal(normals[i], worldRotation, camRotation));
                }

                {
                    auto vec = m->getFacesIndices();
                    m_facesIds.insert(m_facesIds.end(), vec.begin(), vec.end());
                }
                {
                    auto vec = m->getNormalsIndices();
                    m_normalsIds.insert(m_normalsIds.end(), vec.begin(), vec.end());
                }
                {
                    auto vec = m->getFacesColors();
                    m_facesColors.insert(m_facesColors.end(), vec.begin(), vec.end());
                }


                //refIdFaces += m->getVertices().size();
                //refIdNormals += m->getNormals().size();
            }
            m_drawList_next.clear();
            workerLoop_prepareFaces(refIdFaces, refIdNormals);

            m_facesIds.clear();
            m_normalsIds.clear();
            m_facesColors.clear();
        }

        void SoftwareRenderer::workerLoop_prepareFaces(unsigned refIdFaces, unsigned refIdNormals) {
            
            float focalLength = calculateFocalLength(m_engineState->fov, m_engineState->windowWidth);
            float halfWidth = m_engineState->windowWidth / 2.0f;
            float halfHeight = m_engineState->windowHeight / 2.0f;

            for (unsigned i = 0; i < m_facesIds.size() - 2; i += 3) {
                std::vector<Vector3f> faceVerts = {
                    viewSpaceVertices[m_facesIds[i]],
                    viewSpaceVertices[m_facesIds[i + 1]],
                    viewSpaceVertices[m_facesIds[i + 2]]
                };
                std::vector<Vector3f> faceNormals = {
                    verticesNormals[m_normalsIds[i]],
                    verticesNormals[m_normalsIds[i + 1]],
                    verticesNormals[m_normalsIds[i + 2]]
                };

                if (!backfaceCulling(faceVerts, faceNormals, m_engineState->cameraPosition)) continue;

                for (std::vector<Vector3f> clipped : Rendering::clipPolygon(faceVerts, m_engineState->nearClipPlane))
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

                    m_drawList_next.push_back(RenderFace{
                        screenPoints,
                        zCoords,
                        lambertianShading(faceNormals, sf::Color::White), // m_facesColors[(refIdFaces+i)/3]
                        zSum / (float)clipped.size()
                        });
                }
            }
            std::cout << m_drawList_next.size() << "\n";
        }

        void SoftwareRenderer::renderZBuffer() {
            auto startPrep = std::chrono::system_clock::now();
            prepareFaces();
            auto endPrep = std::chrono::system_clock::now();

            std::chrono::duration<double> elapsed_seconds_prep = endPrep - startPrep;

            std::swap(m_drawList, m_drawList_next);

            clearBuffers();

            {
                std::lock_guard<std::mutex> lock(m_syncMtx);
                m_nActiveThreads = m_workers.size();
                m_currentFrameId++;
            }

            auto startDraw = std::chrono::system_clock::now();

            {
                std::unique_lock<std::mutex> lock(m_syncMtx);
                m_cvStart.notify_all();
            }

            {
                std::unique_lock<std::mutex> lock(m_syncMtx);
                m_cvDone.wait(lock, [this] { return m_nActiveThreads == 0; });
            }

            m_renderTexture.update(reinterpret_cast<const uint8_t*>(m_colorBuffer.data()));
            m_window->clear(sf::Color::Black);
            m_window->draw(m_renderSprite);

            auto endDraw = std::chrono::system_clock::now();

            std::chrono::duration<double> elapsed_seconds_draw = endDraw - startDraw;

            std::cout << "prepare time: " << elapsed_seconds_prep.count() << "s"
                << " - draw time" << elapsed_seconds_draw.count() << "s"
                << std::endl;
        }

        void SoftwareRenderer::workerLoop_zBuffer(unsigned threadId, unsigned nbThreads) {
            int sliceHeight = m_engineState->windowHeight / nbThreads;
            int threadStartY = threadId * sliceHeight;
            int threadEndY = (threadId == nbThreads - 1) ? (m_engineState->windowHeight - 1) : (threadStartY + sliceHeight - 1);

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
                    float faceMinY = std::min({ face.points[0].y, face.points[1].y, face.points[2].y });
                    float faceMaxY = std::max({ face.points[0].y, face.points[1].y, face.points[2].y });

                    int faceStartY = std::max(threadStartY, (int)std::floor(faceMinY));
                    int faceEndY = std::min(threadEndY, (int)std::ceil(faceMaxY));

                    if (faceStartY > faceEndY) continue;

                    float faceMinX = std::min({ face.points[0].x, face.points[1].x, face.points[2].x });
                    float faceMaxX = std::max({ face.points[0].x, face.points[1].x, face.points[2].x });
                    int faceStartX = std::max(0, (int)std::floor(faceMinX));
                    int faceEndX = std::min((int)m_engineState->windowWidth - 1, (int)std::ceil(faceMaxX));

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

        void SoftwareRenderer::drawTriangle(const RenderFace& triangle, int startX, int endX, int startY, int endY) {
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

        bool SoftwareRenderer::putPixel(int x, int y, float z, sf::Color color) {
            if (x < 0 || x >= m_engineState->windowWidth || y < 0 || y >= m_engineState->windowHeight) {
                return false;
            }
            int i = y * m_engineState->windowWidth + x;

            bool isNewZ = z < depthBuffers[i];
            if (isNewZ) {
                depthBuffers[i] = z;
                m_colorBuffer[i] = colorToUint32(color);
            }
            return isNewZ;
        }

        void SoftwareRenderer::clearBuffers() {
            std::fill(m_colorBuffer.begin(), m_colorBuffer.end(), 0xff949494);
            std::fill(depthBuffers.begin(), depthBuffers.end(), 10000.0f);
        }

        void SoftwareRenderer::renderPainter() {
            prepareFaces();

            paintersAlgorithm(m_drawList);

            m_window->clear(sf::Color(148, 140, 140, 1));

            for (const auto& face : m_drawList) {
                sf::ConvexShape poly(face.points.size());
                for (size_t i = 0; i < face.points.size(); ++i) poly.setPoint(i, face.points[i]);
                poly.setFillColor(face.color);
                m_window->draw(poly);
            }
        }

        inline void SoftwareRenderer::paintersAlgorithm(std::vector<RenderFace>& facesToDraw) {
            std::sort(facesToDraw.begin(), facesToDraw.end(), [](const RenderFace& a, const RenderFace& b) {
                return a.avgZ > b.avgZ;
                });
        }
    }
}
