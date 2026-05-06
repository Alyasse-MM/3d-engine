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

        void SoftwareRenderer::prepareFacesToDraw() {
            m_drawList = {};

            auto worldRotation = Matrix3<float>::getRotationX(toRadians(m_engineState.modelAngleX)) * Matrix3<float>::getRotationY(toRadians(m_engineState.modelAngleY));
            auto camRotation = Matrix3<float>::getRotationY(toRadians(-m_engineState.cameraYaw));

            std::vector<Vector3<float>> viewSpaceVertices;
            std::vector<Vector3<float>> verticesNormals;
            for (const auto& m : m_scene->m_meshes) {
                auto vertices = m->getVertices();
                auto normals = m->getNormals();
                for (int i = 0; i < vertices.size(); i++) {
                    viewSpaceVertices.push_back(worldToView(vertices[i], worldRotation, camRotation, m_engineState.cameraPosition));
                }
                for (int i = 0; i < normals.size(); i++) {
                    verticesNormals.push_back(worldToViewNormal(normals[i], worldRotation, camRotation));
                }
            }

            float focalLength = calculateFocalLength(m_engineState.fov, m_engineState.windowWidth);
            float halfWidth = m_engineState.windowWidth / 2.0f;
            float halfHeight = m_engineState.windowHeight / 2.0f;

            unsigned refIdFaces = 0;
            unsigned refIdNormals = 0;
            for (const auto& m : m_scene->m_meshes) {
                std::vector<unsigned> facesIds = m->getFacesIndices();
                std::vector<unsigned> normalsIds = m->getNormalsIndices();
                for (unsigned i = 0; i < facesIds.size() - 2; i += 3) {
                    std::vector<Vector3f> faceVerts = {
                        viewSpaceVertices[refIdFaces + facesIds[i]],
                        viewSpaceVertices[refIdFaces + facesIds[i + 1]],
                        viewSpaceVertices[refIdFaces + facesIds[i + 2]]
                    };
                    std::vector<Vector3f> faceNormals = {
                        verticesNormals[refIdNormals + normalsIds[i]],
                        verticesNormals[refIdNormals + normalsIds[i + 1]],
                        verticesNormals[refIdNormals + normalsIds[i + 2]]
                    };

                    if (!backfaceCulling(faceVerts, faceNormals, m_engineState.cameraPosition)) continue;

                    for (std::vector<Vector3f> clipped : Rendering::clipPolygon(faceVerts, m_engineState.nearClipPlane))
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
                            lambertianShading(faceNormals, sf::Color::White), // m->getFaceColor(i/3)
                            zSum / (float)clipped.size()
                            });
                    }
                }
                refIdFaces += m->getVertices().size();
                refIdNormals += m->getNormals().size();
            }
        }
    }
}
