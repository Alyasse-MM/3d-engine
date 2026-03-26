#pragma once
#include "Vector3.h"
#include "Matrix3.h"
#include <SFML/System/Vector2.hpp>

namespace Maths {
    template <typename T>
    Matrix3<T> operator*(const Matrix3<T>& A, const Matrix3<T>& B) {
        Matrix3<T> result;
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                result(r, c) = A.m[r * 3 + 0] * B.m[0 * 3 + c] +
                    A.m[r * 3 + 1] * B.m[1 * 3 + c] +
                    A.m[r * 3 + 2] * B.m[2 * 3 + c];
            }
        }
        return result;
    }

    template <typename T>
    Vector3<T> operator*(const Matrix3<T>& mat, const Vector3<T>& v) {
        return {
            mat.m[0] * v.x + mat.m[1] * v.y + mat.m[2] * v.z,
            mat.m[3] * v.x + mat.m[4] * v.y + mat.m[5] * v.z,
            mat.m[6] * v.x + mat.m[7] * v.y + mat.m[8] * v.z
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

    inline bool backfaceCulling(const std::vector<Vector3<float>>& faceVerts) {
        if (faceVerts.size() >= 3) {
            Vector3<float> normal = cross(faceVerts[1] - faceVerts[0], faceVerts[2] - faceVerts[0]);
            return (dot(normal, faceVerts[0]) >= 0);
        }
        return false;
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
        const Vector3<float>& cameraPos)
    {
        Vector3<float> world = modelRotation * v;
        Vector3<float> translated = world - cameraPos;
        return viewRotation * translated;
    }
};
