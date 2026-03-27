#include "ZBufferRenderer.hpp"
#include "maths/Math.hpp"
#include "graphics/Clipping.hpp"
#include <algorithm>

using namespace Maths;

void ZBufferRenderer::render() {
    Matrix3<float> modelRot = Matrix3<float>::getRotationX(toRadians(enginestate.model_angle_x)) * Matrix3<float>::getRotationY(toRadians(enginestate.model_angle_y));
    Matrix3<float> viewRot = Matrix3<float>::getRotationY(toRadians(-enginestate.camera_yaw));

    float halfW = window.getSize().x / 2.0f;
    float halfH = window.getSize().y / 2.0f;

    float focalLength = Maths::calculateFocalLength(enginestate.fov, enginestate.windowWidth);

    std::vector<Vector3<float>> viewSpaceVertices;
    for (const auto& v : scene->vertices) {
        viewSpaceVertices.push_back(worldToView(v, modelRot, viewRot, enginestate.camera_pos));
    }

    std::vector<RenderFace> drawList;

    for (const auto& face : scene->faces) {
        std::vector<Vector3<float>> faceVerts;
        for (int idx : face.indices) faceVerts.push_back(viewSpaceVertices[idx]);

        if (backfaceCulling(faceVerts)) continue;

        std::vector<Vector3<float>> clipped = Graphics::clipPolygon(faceVerts, enginestate.z_near);
        if (clipped.size() < 3) continue;

        std::vector<sf::Vector2f> screenPoints;
        float zSum = 0;
        for (const auto& v : clipped) {
            screenPoints.push_back(perspectiveProjection(v, focalLength, halfW, halfH));
            zSum += v.z;
        }

        drawList.push_back({ screenPoints, face.color, zSum / (float)clipped.size() });
    }

    // paintersAlgorithm(drawList);

    for (const auto& f : drawList) {
        sf::ConvexShape poly(f.points.size());
        for (size_t i = 0; i < f.points.size(); ++i) poly.setPoint(i, f.points[i]);
        poly.setFillColor(f.color);
        window.draw(poly);
    }
}

void ZBufferRenderer::clear() {
    std::fill(color_buffer.begin(), color_buffer.end(), 0x000000);
    std::fill(z_buffer.begin(), z_buffer.end(), 1.0f);
}