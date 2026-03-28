#include "ZBufferRenderer.hpp"
#include "maths/Math.hpp"
#include "graphics/Clipping.hpp"
#include <algorithm>
#include <omp.h>

using namespace Maths;

void ZBufferRenderer::render() {
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

    for (const Face& face : scene->faces) {
        for (size_t i = 1; i < face.indices.size() - 1; ++i) {
            Face tri = face;
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
            if (clipped.size() < 3) continue;

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

    clear();

    for (const RenderFace& f : drawList) {
        drawTriangle(f);
    }

    renderTexture.update(reinterpret_cast<const uint8_t*>(color_buffer.data()));
    window.clear(sf::Color::Black);
    window.draw(renderSprite);
}

bool ZBufferRenderer::putPixel(int x, int y, float z, sf::Color color) {
    if (x < 0 || x >= enginestate.windowWidth || y < 0 || y >= enginestate.windowHeight) {
        return false;
    }
    int index = y * enginestate.windowWidth + x;

    bool isNewZ = z < z_buffer[index];
    if (isNewZ) {
        z_buffer[index] = z;
        color_buffer[index] = colorToUint32(color);
    }
    return isNewZ;
}

void ZBufferRenderer::drawTriangle(const RenderFace& triangle) {
    if (triangle.points.size() != 3) {
        std::cout << "not a triangle" << std::endl;
        return;
    }
    float minX = std::min({ triangle.points[0].x, triangle.points[1].x, triangle.points[2].x });
    float maxX = std::max({ triangle.points[0].x, triangle.points[1].x, triangle.points[2].x });
    float minY = std::min({ triangle.points[0].y, triangle.points[1].y, triangle.points[2].y });
    float maxY = std::max({ triangle.points[0].y, triangle.points[1].y, triangle.points[2].y });

    int leftX = std::max(0, (int)std::floor(minX));
    int rightX = std::min((int)enginestate.windowWidth - 1, (int)std::ceil(maxX));
    int topY = std::max(0, (int)std::floor(minY));
    int bottomY = std::min((int)enginestate.windowHeight - 1, (int)std::ceil(maxY));

    float triangleArea = perpProduct(triangle.points[0], triangle.points[1], triangle.points[2]);

    if (std::abs(triangleArea) < 0.000001f) return;

    for (int y = topY; y <= bottomY; ++y) {
        for (int x = leftX; x <= rightX; ++x) {
            sf::Vector2f p(x, y);

            float vAB = perpProduct(triangle.points[0], triangle.points[1], p);
            float vBC = perpProduct(triangle.points[1], triangle.points[2], p);
            float vCA = perpProduct(triangle.points[2], triangle.points[0], p);

            if (insideTriangle(vAB, vBC, vCA)) {
                float wC = vAB / triangleArea;
                float wA = vBC / triangleArea;
                float wB = vCA / triangleArea;

                float z = (wA * triangle.zValues[0]) +
                    (wB * triangle.zValues[1]) +
                    (wC * triangle.zValues[2]);

                putPixel(x, y, z, triangle.color);
            }
        }
    }
}

void ZBufferRenderer::clear() {
    std::fill(color_buffer.begin(), color_buffer.end(), 0xff949494);
    std::fill(z_buffer.begin(), z_buffer.end(), 10000.0f);
}