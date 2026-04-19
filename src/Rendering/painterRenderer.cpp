#include "painterRenderer.hpp"
#include "Maths/Maths.hpp"
#include "Rendering/Clipping.hpp"
#include <algorithm>
#include <iostream>

namespace al3d
{
    using namespace Maths;

    inline void PainterRenderer::paintersAlgorithm(std::vector<RenderFace>& facesToDraw) {
        std::sort(facesToDraw.begin(), facesToDraw.end(), [](const RenderFace& a, const RenderFace& b) {
            return a.avgZ > b.avgZ;
            });
    }

    void PainterRenderer::render() {
        auto modelRotation = Matrix3<float>::getRotationX(toRadians(m_engineState.modelAngleX)) * Matrix3<float>::getRotationY(toRadians(m_engineState.modelAngleY));
        auto viewRotation = Matrix3<float>::getRotationY(toRadians(-m_engineState.cameraYaw));

        std::vector<Vector3<float>> viewSpaceVertices;
        for (const auto& v : m_scene->m_vertices) {
            viewSpaceVertices.push_back(worldToView(v, modelRotation, viewRotation, m_engineState.cameraPosition));
        }

        std::vector<Face> m_faces;

        for (const auto& face : m_scene->m_faces) {
            for (size_t i = 1; i < face.indices.size() - 1; ++i) {
                auto tri = face;
                tri.indices = { face.indices[0], face.indices[i], face.indices[i + 1] };
                m_faces.push_back(tri);
            }
        }

        float halfWidth = m_engineState.windowWidth / 2.0f;
        float halfHeight = m_engineState.windowHeight / 2.0f;
        float focalLength = Maths::calculateFocalLength(m_engineState.fov, m_engineState.windowWidth);

        std::vector<RenderFace> facesToDraw;

        for (const auto& face : m_faces) {
            std::vector<Vector3<float>> faceVerts;
            for (unsigned i : face.indices) faceVerts.push_back(viewSpaceVertices[i]);

            if (backfaceCulling(faceVerts)) continue;

            for (std::vector<Vector3<float>> clipped : Rendering::clipPolygon(faceVerts, m_engineState.nearClipPlane))
            {
                if (clipped.size() != 3) continue;

                std::vector<sf::Vector2f> screenPoints;
                std::vector<float> zCoords;
                float zSum = 0;
                for (const auto& v : clipped) {
                    screenPoints.push_back(perspectiveProjection(v, focalLength, halfWidth, halfHeight));
                    zCoords.push_back(v.z),
                        zSum += v.z;
                }

                facesToDraw.push_back(RenderFace{
                    screenPoints,
                    zCoords,
                    face.color,
                    zSum / (float)clipped.size()
                    });
            }
        }

        paintersAlgorithm(facesToDraw);

        m_window.clear(sf::Color(148, 140, 140, 1));

        for (const auto& face : facesToDraw) {
            sf::ConvexShape poly(face.points.size());
            for (size_t i = 0; i < face.points.size(); ++i) poly.setPoint(i, face.points[i]);
            poly.setFillColor(face.color);
            m_window.draw(poly);
        }
    }
}