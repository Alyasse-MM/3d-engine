#include "renderer.h"
#include "maths/Math.h"
#include "Clipping.h"

void Renderer::render(sf::RenderWindow& window, EngineState& state, Scene& scene) {
    Matrix3<float> rx = Matrix3<float>::getRotationX(Maths::toRadians(state.angle_x));
    Matrix3<float> ry = Matrix3<float>::getRotationY(Maths::toRadians(state.angle_y));
    Matrix3<float> viewRy = Matrix3<float>::getRotationY(Maths::toRadians(-state.camera_yaw));

    std::vector<Vector3<float>> transformed;
    for (const auto& v : scene.vertices) {
        Vector3<float> world = rx*(ry * v);
        Vector3<float> translated = world - state.camera_pos;
        Vector3<float> view = viewRy * translated;
        transformed.push_back(view);
    }

    std::vector<RenderFace> drawList;
    float focalLength = 400.0f;
    float halfW = window.getSize().x / 2.0f;
    float halfH = window.getSize().y / 2.0f;

    for (const auto& face : scene.faces) {
        std::vector<Vector3<float>> faceVerts;
        for (int idx : face.indices) {
            faceVerts.push_back(transformed[idx]);
        }

        if (faceVerts.size() >= 3) {
            Vector3<float> v1 = faceVerts[1] - faceVerts[0];
            Vector3<float> v2 = faceVerts[2] - faceVerts[0];
            Vector3<float> normal = cross(v1, v2);

            if (normal.z >= 0) continue;
        }

        std::vector<Vector3<float>> clippedVerts = Graphics::clipPolygon(faceVerts);

        if (clippedVerts.size() < 3) continue;

        std::vector<sf::Vector2f> screenPoints;
        float zSum = 0;

        for (const auto& v : clippedVerts) {
            float x = (v.x / v.z) * focalLength + halfW;
            float y = (v.y / v.z) * focalLength + halfH;

            screenPoints.push_back({ x, y });
            zSum += v.z;
        }

        drawList.push_back({ screenPoints, face.color, zSum / clippedVerts.size() });
    }

    std::sort(drawList.begin(), drawList.end(), [](const RenderFace& a, const RenderFace& b) {
        return a.avgZ > b.avgZ;
        });

    for (const auto& item : drawList) {
        sf::ConvexShape poly;
        poly.setPointCount(item.points.size());
        for (size_t i = 0; i < item.points.size(); ++i) {
            poly.setPoint(i, item.points[i]);
        }
        poly.setFillColor(item.color);
        poly.setOutlineColor(sf::Color::Black);
        poly.setOutlineThickness(1.0f);
        window.draw(poly);
    }
}