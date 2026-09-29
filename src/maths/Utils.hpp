#pragma once
#include "Vector3.hpp"
#include "Matrix3.hpp"
#include "Matrix4.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

namespace al3d
{
    namespace Maths {

        template <typename T>
        Matrix4<T> operator*(const Matrix4<T>& A, const Matrix4<T>& B) {
            Matrix4<float> result;
            for (int row = 0; row < 4; ++row) {
                for (int col = 0; col < 4; ++col) {
                    result.data[row * 4 + col] =
                        A.data[row * 4 + 0] * B.data[0 * 4 + col] +
                        A.data[row * 4 + 1] * B.data[1 * 4 + col] +
                        A.data[row * 4 + 2] * B.data[2 * 4 + col] +
                        A.data[row * 4 + 3] * B.data[3 * 4 + col];
                }
            }
            return result;
        }

        template <typename T>
        Vector3<T> operator*(const Matrix4<T> A, const Vector3<T>& B) {
            T x = A.data[0] * B.x + A.data[1] * B.y + A.data[2] * B.z + A.data[3] * 1;
            T y = A.data[4] * B.x + A.data[5] * B.y + A.data[6] * B.z + A.data[7] * 1;
            T z = A.data[8] * B.x + A.data[9] * B.y + A.data[10] * B.z + A.data[11] * 1;
            T w = A.data[12] * B.x + A.data[13] * B.y + A.data[14] * B.z + A.data[15] * 1;

            if (w != 0 && w != 1) {
                T invW = 1 / w;
                return Vector3<T>{ x* invW, y* invW, z* invW };
            }
            return Vector3<T>{ x, y, z };
        }

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

        inline bool backfaceCulling(const Vector3<float> vertices[3], const Vector3<float> normals[3]) {
            Vector3<float> faceNormal = (normals[0] + normals[1] + normals[2]) / 3.0f;
            Vector3<float> normalizedFaceNormal = al3d::Maths::Vector3<float>::normalize(faceNormal);

            Vector3<float> faceCenter = (vertices[0] + vertices[1] + vertices[2]) / 3.0f;
            Vector3<float> perspectiveLookDir = al3d::Maths::Vector3<float>::normalize(faceCenter);

            return (dot(normalizedFaceNormal, perspectiveLookDir) < 0);
        }

        inline sf::Vector2f perspectiveProjection(const Vector3<float>& v, float focalLength, float centerX, float centerY) {
            return {
                (v.x / v.z) * focalLength + centerX,
                (-v.y / v.z) * focalLength + centerY
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
            const Matrix4<float>& modelRotation,
            const Matrix4<float>& viewRotation,
            const Vector3<float>& cameraPosition)
        {
            Vector3<float> world = modelRotation * v;
            Vector3<float> translated = world - cameraPosition;
            return viewRotation * translated;
        }
        
        inline Vector3<float> worldToViewNormal(const Vector3<float>& n,
            const Matrix4<float>& modelRotation,
            const Matrix4<float>& viewRotation)
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

        inline sf::Color lambertianShading(float minDarkness, const Vector3<float> normals[3], sf::Color baseColor, const Vector3<float> lightDir) {
            Vector3<float> faceNormal = al3d::Maths::Vector3<float>::normalize((normals[0] + normals[1] + normals[2]) / 3.0f);
            
            Vector3<float> normalLightDir = al3d::Maths::Vector3<float>::normalize(lightDir);

            float intensity = dot(faceNormal, normalLightDir);

            intensity = (minDarkness+std::max(0.0f, intensity))/(1.0f+minDarkness);

            return sf::Color(
                static_cast<uint8_t>(baseColor.r * intensity),
                static_cast<uint8_t>(baseColor.g * intensity),
                static_cast<uint8_t>(baseColor.b * intensity)
            );
        }
    };
}