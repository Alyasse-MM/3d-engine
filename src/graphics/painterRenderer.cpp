#include "painterRenderer.hpp"
#include "maths/Math.hpp"
#include "graphics/Clipping.hpp"
#include <algorithm>
#include <iostream>

using namespace Maths;

inline void PainterRenderer::paintersAlgorithm(std::vector<RenderFace>& drawList) {
    std::sort(drawList.begin(), drawList.end(), [](const RenderFace& a, const RenderFace& b) {
        return a.avgZ > b.avgZ;
        });
}

void PainterRenderer::render() {
    Matrix3<float> modelRot = Matrix3<float>::getRotationX(toRadians(enginestate.model_angle_x)) * Matrix3<float>::getRotationY(toRadians(enginestate.model_angle_y));
    Matrix3<float> viewRot = Matrix3<float>::getRotationY(toRadians(-enginestate.camera_yaw));

    float halfW = enginestate.windowWidth / 2.0f;
    float halfH = enginestate.windowHeight / 2.0f;

    float focalLength = Maths::calculateFocalLength(enginestate.fov, enginestate.windowWidth);

    std::vector<Vector3<float>> viewSpaceVertices;
    for (const auto& v : scene->vertices) {
        viewSpaceVertices.push_back(worldToView(v, modelRot, viewRot, enginestate.camera_pos));
    }

    std::vector<Face> triangles;

    for (const auto& face : scene->faces) {
        for (size_t i = 1; i < face.indices.size() - 1; ++i) {
            auto tri = face;
            tri.indices = { face.indices[0], face.indices[i], face.indices[i + 1] };
            triangles.push_back(tri);
        }
    }

    std::vector<RenderFace> drawList;

    for (const auto& face : triangles) {
        std::vector<Vector3<float>> faceVerts;
        for (int idx : face.indices) faceVerts.push_back(viewSpaceVertices[idx]);

        if (backfaceCulling(faceVerts)) continue;

        for (std::vector<Vector3<float>> clipped : Graphics::clipPolygon(faceVerts, enginestate.z_near))
        {
            if (clipped.size() != 3) continue;

            std::vector<sf::Vector2f> screenPoints;
            std::vector<float> zCoords;
            float zSum = 0;
            for (const auto& v : clipped) {
                screenPoints.push_back(perspectiveProjection(v, focalLength, halfW, halfH));
                zCoords.push_back(v.z),
                    zSum += v.z;
            }

            drawList.push_back(RenderFace{
                screenPoints,
                zCoords,
                face.color,
                zSum / (float)clipped.size()
                });
        }
    }

    paintersAlgorithm(drawList);

    window.clear(sf::Color(148,140,140,1));

    for (const auto& f : drawList) {
        sf::ConvexShape poly(f.points.size());
        for (size_t i = 0; i < f.points.size(); ++i) poly.setPoint(i, f.points[i]);
        poly.setFillColor(f.color);
        window.draw(poly);
    }
}