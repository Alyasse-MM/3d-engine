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

        Vector3f SoftwareRenderer::getLightNormal(LightDirection l) {
            switch (l) {
            case(1):
                return { 0,0,1 };
                break;
            case(2):
                return { 0,0,-1 };
                break;
            case(3):
                return { 1,0,0 };
                break;
            case(4):
                return { -1,0,0};
                break;
            case(5):
                return { 0,1,0 };
                break;
            case(6):
                return { 0,-1,0 };
                break;
            default:
                return { 0,0,1 };
            }
        }

        void SoftwareRenderer::prepareFaces() {
            auto worldRotation = Matrix4<float>::getRotationX(toRadians(m_engineState->modelAngleX)) * Matrix4<float>::getRotationY(toRadians(m_engineState->modelAngleY));
            auto camRotation = Matrix4<float>::getRotationY(toRadians(-m_engineState->cameraYaw));
            auto camNormal = camRotation * m_engineState->cameraNormal;
            m_drawList_next.clear();

            viewSpaceVertices.clear();
            verticesNormals.clear();
            m_facesIds.clear();
            m_normalsIds.clear();
            m_facesColors.clear();

            std::vector<Vector3f> vertices, normals;
            unsigned refIdFaces{ 0 }, refIdNormals{ 0 };
            for (const auto& m : m_scene->m_meshInstances) {
                vertices = m->getMesh()->getVertices();
                normals = m->getMesh()->getNormals();
                auto instanceMatrix4 = m->getTransformMatrix4();
                auto instanceMatrix3 = m->getTransformMatrix3();

                for (int i = 0; i < vertices.size(); i++) {
                    vertices[i]=worldToView(instanceMatrix4 * vertices[i], worldRotation, camRotation, m_engineState->cameraPosition);
                }
                for (int i = 0; i < normals.size(); i++) {
                    normals[i]=worldToViewNormal(instanceMatrix3 * normals[i], worldRotation, camRotation);
                }
                auto facesIds = m->getMesh()->getFacesIndices();
                auto normalsIds = m->getMesh()->getNormalsIndices();
                sf::Color meshColor = m->getMesh()->getBaseColor();

                for (unsigned i = 0; i < facesIds.size() - 2; i += 3) {
                    Vector3f faceVerts[3] = {
                    vertices[facesIds[i]],
                    vertices[facesIds[i + 1]],
                    vertices[facesIds[i + 2]]
                    };
                    Vector3f faceNormals[3] = {
                        normals[normalsIds[i]],
                        normals[normalsIds[i + 1]],
                        normals[normalsIds[i + 2]]
                    };

                    if (!backfaceCulling(faceVerts, faceNormals)) continue;

                    for (std::vector<Vector3f> clipped : Rendering::clipPolygon(faceVerts, m_engineConfig->nearClipPlane))
                    {
                        if (clipped.size() != 3) continue;

                        std::vector<sf::Vector2f> screenPoints;
                        std::vector<float> zCoords;
                        float zSum = 0;
                        for (const auto& vertex : clipped) {
                            screenPoints.push_back(perspectiveProjection(vertex, m_engineConfig->focalLength, m_engineConfig->halfWidth, m_engineConfig->halfHeight));
                            zCoords.push_back(vertex.z),
                                zSum += vertex.z;
                        }

                        m_drawList_next.push_back(RenderFace{
                            screenPoints,
                            zCoords,
                            lambertianShading(m_engineConfig->minDarkness, faceNormals, meshColor, m_engineConfig->lightDirections), // m_facesColors[(refIdFaces+i)/3]
                            zSum / (float)clipped.size()
                            });
                    }
                }
            }
        }

        void SoftwareRenderer::renderZBuffer() {
            prepareFaces();

            std::swap(m_drawList, m_drawList_next);

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
            m_window->clear(sf::Color::Black);
            m_window->draw(m_renderSprite);
        }

        void SoftwareRenderer::workerLoop_zBuffer(unsigned threadId, unsigned nbThreads) {
            int sliceHeight = m_engineConfig->windowHeight / nbThreads;
            int threadStartY = threadId * sliceHeight;
            int threadEndY = (threadId == nbThreads - 1) ? (m_engineConfig->windowHeight - 1) : (threadStartY + sliceHeight - 1);

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
                    int faceEndX = std::min((int)m_engineConfig->windowWidth - 1, (int)std::ceil(faceMaxX));

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
            if (x < 0 || x >= m_engineConfig->windowWidth || y < 0 || y >= m_engineConfig->windowHeight) {
                return false;
            }
            int i = y * m_engineConfig->windowWidth + x;

            bool isNewZ = z < m_depthBuffers[i];
            if (isNewZ) {
                m_depthBuffers[i] = z;
                m_colorBuffer[i] = colorToUint32(color);
            }
            return isNewZ;
        }

        void SoftwareRenderer::clearBuffers() {
            std::fill(m_colorBuffer.begin(), m_colorBuffer.end(), 0xff949494);
            std::fill(m_depthBuffers.begin(), m_depthBuffers.end(), 10000.0f);
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
