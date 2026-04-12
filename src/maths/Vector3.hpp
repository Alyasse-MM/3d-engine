#pragma once

namespace al3d
{
    namespace Maths {
        template <typename T>
        struct Vector3 {
            T x, y, z;

            Vector3 operator+(const Vector3& v) const {
                return { x + v.x, y + v.y, z + v.z };
            }

            Vector3 operator-(const Vector3& v) const {
                return { x - v.x, y - v.y, z - v.z };
            }

            Vector3 operator*(T scalar) const {
                return { x * scalar, y * scalar, z * scalar };
            }
        };
    }
}