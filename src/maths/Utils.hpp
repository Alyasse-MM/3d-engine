#pragma once
#include "Vector3.hpp"
#include "Matrix3.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

namespace al3d
{
    namespace Maths {
        template <typename T>
        Matrix3<T> operator*(const Matrix3<T>& A, const Matrix3<T>& B) {
            Matrix3<T> result;
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    result(row, col) = A.data[row * 3 + 0] * B.data[0 * 3 + col] +
                        A.data[row * 3 + 1] * B.data[1 * 3 + col] +
                        A.data[row * 3 + 2] * B.data[2 * 3 + col];
                }
            }
            return result;
        }

        template <typename T>
        Vector3<T> operator*(const Matrix3<T>& mat, const Vector3<T>& v) {
            return {
                mat.data[0] * v.x + mat.data[1] * v.y + mat.data[2] * v.z,
                mat.data[3] * v.x + mat.data[4] * v.y + mat.data[5] * v.z,
                mat.data[6] * v.x + mat.data[7] * v.y + mat.data[8] * v.z
            };
        }

        template <typename T>
        inline T dot(const Vector3<T>& a, const Vector3<T>& b) {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        template <typename T>
        inline Vector3<T> cross(const Vector3<T>& a, const Vector3<T>& b) {
            return {
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            };
        }

        inline float toRadians(float degrees) {
            return degrees * 3.14159f / 180.0f;
        };

        inline bool backfaceCulling(const Vector3<float> vertices[3],
            const Vector3<float> normals[3],
            const Vector3<float>& cameraPos) {

            Vector3<float> faceNormal = (normals[0] + normals[1] + normals[2]) / 3.0f;

            Vector3<float> faceCenter = (vertices[0] + vertices[1] + vertices[2]) / 3.0f;

            Vector3<float> viewVector = cameraPos - faceCenter;

            // 3. Dot Product: If > 0, the face is pointing toward the camera
            return (dot(faceNormal, viewVector) > 0);
        }

        inline sf::Vector2f perspectiveProjection(const Vector3<float>& v, float focalLength, float centerX, float centerY) {
            return {
                (v.x / v.z) * focalLength + centerX,
                (v.y / v.z) * focalLength + centerY
            };
        }

        template <typename T, typename U>
        inline T lerp(const T& a, const T& b, U t) {
            return a + (b - a) * t;
        }

        inline float calculateFocalLength(float fovDegrees, unsigned windowWidth) {
            float fovRadians = toRadians(fovDegrees);
            return (static_cast<float>(windowWidth) * 0.5f) / std::tan(fovRadians * 0.5f);
        }

        inline Vector3<float> worldToView(const Vector3<float>& v,
            const Matrix3<float>& modelRotation,
            const Matrix3<float>& viewRotation,
            const Vector3<float>& cameraPosition)
        {
            Vector3<float> world = modelRotation * v;
            Vector3<float> translated = world - cameraPosition;
            return viewRotation * translated;
        }
        
        inline Vector3<float> worldToViewNormal(const Vector3<float>& n,
            const Matrix3<float>& modelRotation,
            const Matrix3<float>& viewRotation)
        {
            Vector3<float> worldNormal = modelRotation * n;

            Vector3<float> viewNormal = viewRotation * worldNormal;

            return al3d::Maths::Vector3<float>::normalize(viewNormal);
        }

        inline float perpProduct(sf::Vector2f start, sf::Vector2f end, sf::Vector2f point) {
            return ((point.x - end.x) * (start.y - end.y) - (start.x - end.x) * (point.y - end.y));
        }

        inline bool insideTriangle(float vAB, float vBC, float vCA) {
            return ((vAB <= 0 && vBC <= 0 && vCA <= 0) || (vAB >= 0 && vBC >= 0 && vCA >= 0));
        }

        inline sf::Color lambertianShading(float minDarkness, const Vector3<float> normals[3], sf::Color baseColor) {
            Vector3<float> faceNormal = (normals[0] + normals[1] + normals[2]) / 3.0f;
            
            Vector3<float> lightDir = al3d::Maths::Vector3<float>::normalize(Vector3<float>{0.0f, 0.0f, -1.0f});

            float intensity = dot(faceNormal, lightDir);

            intensity = (minDarkness+std::max(0.0f, intensity))/(1.0f+minDarkness);

            return sf::Color(
                static_cast<uint8_t>(baseColor.r * intensity),
                static_cast<uint8_t>(baseColor.g * intensity),
                static_cast<uint8_t>(baseColor.b * intensity)
            );
        }
    };
}