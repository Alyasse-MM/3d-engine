#pragma once
#include "Vector3.h"
#include "Matrix3.h"

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

namespace Maths {
    inline float toRadians(float degrees) {
        return degrees * 3.14159f / 180.0f;
    };
};