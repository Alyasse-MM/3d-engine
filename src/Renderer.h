#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "State.h"
#include "Scene.h"
#include "Maths.h"

struct RenderFace {
    std::vector<sf::Vector2f> points;
    sf::Color color;
    float avgZ;
};

class Renderer {
public:
    void render(sf::RenderWindow& window, EngineState& state, Scene& scene) {
        Matrix3 rx = Maths::getRotationX(Maths::toRadians(state.angle_x));
        Matrix3 ry = Maths::getRotationY(Maths::toRadians(state.angle_y));
        Matrix3 viewRy = Maths::getRotationY(Maths::toRadians(-state.camera_yaw));

        std::vector<Vec3> transformed;
        for (const auto& v : scene.vertices) {
            Vec3 world = Maths::matMul(rx, Maths::matMul(ry, v));
            Vec3 translated = world - state.camera_pos;
            Vec3 view = Maths::matMul(viewRy, translated);
            transformed.push_back(view);
        }

        std::vector<RenderFace> drawList;
        float focalLength = 400.0f;
        float halfW = window.getSize().x / 2.0f;
        float halfH = window.getSize().y / 2.0f;

        for (const auto& face : scene.faces) {
            std::vector<Vec3> faceVerts;
            for (int idx : face.indices) {
                faceVerts.push_back(transformed[idx]);
            }

            if (faceVerts.size() >= 3) {
                Vec3 v1 = faceVerts[1] - faceVerts[0];
                Vec3 v2 = faceVerts[2] - faceVerts[0];
                Vec3 normal = Maths::crossProduct(v1, v2);
                
                 if (normal.z >= 0) continue; 
            }

            std::vector<Vec3> clippedVerts = Maths::clipPolygon(faceVerts);

            if (clippedVerts.size() < 3) continue;

            std::vector<sf::Vector2f> screenPoints;
            float zSum = 0;

            for (const auto& v : clippedVerts) {
                float x = (v.x / v.z) * focalLength + halfW;
                float y = (v.y / v.z) * focalLength + halfH;
                
                screenPoints.push_back({x, y});
                zSum += v.z;
            }

            drawList.push_back({screenPoints, face.color, zSum / clippedVerts.size()});
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
};