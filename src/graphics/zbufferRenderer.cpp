#include "ZBufferRenderer.hpp"
#include "maths/Math.hpp"
#include "graphics/Clipping.hpp"
#include <algorithm>
#include <omp.h>

using namespace Maths;

void ZBufferRenderer::workerLoop(int threadID, int numThreads) {
    int sliceHeight = enginestate.windowHeight / numThreads;
    int myStartY = threadID * sliceHeight;
    int myEndY = (threadID == numThreads - 1) ? (enginestate.windowHeight - 1) : (myStartY + sliceHeight - 1);

    std::unique_lock<std::mutex> lock(printMtx);
    std::cout << threadID << " : " << myStartY << " ; " << myEndY << std::endl;
    lock.unlock();

    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv_start.wait(lock, [this] { return frame_ready || stop_threads; });
        if (stop_threads) return;
        lock.unlock();

        std::cout << threadID << "-> do work" << std::endl;
        for (const auto& f : drawList) {
            float minY = std::min({ f.points[0].y, f.points[1].y, f.points[2].y });
            float maxY = std::max({ f.points[0].y, f.points[1].y, f.points[2].y });

            int startY = std::max(myStartY, (int)std::floor(minY));
            int endY = std::min(myEndY, (int)std::ceil(maxY));

            if (startY > endY) continue;

            float minX = std::min({ f.points[0].x, f.points[1].x, f.points[2].x });
            float maxX = std::max({ f.points[0].x, f.points[1].x, f.points[2].x });
            int startX = std::max(0, (int)std::floor(minX));
            int endX = std::min((int)enginestate.windowWidth - 1, (int)std::ceil(maxX));

            drawTriangle(f, startX, endX, startY, endY);
        }

        lock.lock();
        active_threads--;
        if (active_threads == 0) {
            frame_ready = false;
            cv_done.notify_one();
        }
        lock.unlock();
    }
}

void ZBufferRenderer::render() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv_done.wait(lock, [this] { return active_threads == 0; });
        drawList.clear();
    }

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

    {
        std::lock_guard<std::mutex> lock(mtx);
        active_threads = workers.size();
        frame_ready = true;
    }
    std::cout << "notity_all()" << std::endl;
    cv_start.notify_all();

    {
        std::unique_lock<std::mutex> lock(mtx);
        cv_done.wait(lock, [this] { return active_threads == 0; });
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

void ZBufferRenderer::drawTriangle(const RenderFace& triangle, int startX, int endX, int startY, int endY) {
    float triangleArea = perpProduct(triangle.points[0], triangle.points[1], triangle.points[2]);

    if (std::abs(triangleArea) < 0.000001f) return;

    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
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