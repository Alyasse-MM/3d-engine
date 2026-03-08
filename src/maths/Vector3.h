#pragma once

template <typename T>
struct Vector3 {
    T x, y, z;

    Vector3 operator+(const Vector3& other) const {
        return { x + other.x, y + other.y, z + other.z };
    }

    Vector3 operator-(const Vector3& other) const {
        return { x - other.x, y - other.y, z - other.z };
    }

    Vector3 operator*(T scalar) const {
        return { x * scalar, y * scalar, z * scalar };
    }
};