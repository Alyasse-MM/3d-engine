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
            
            Vector3 operator/(T scalar) const {
                return { x / scalar, y / scalar, z / scalar };
            }
            static Vector3<T> normalize(const Vector3<T>& v) {
                T lengthSquared = v.x * v.x + v.y * v.y + v.z * v.z;

                T length = std::sqrt(lengthSquared);

                if (length == 0) {
                    return { 0, 0, 0 };
                }

                T invLength = 1.0f / length;
                return { v.x * invLength, v.y * invLength, v.z * invLength };
            }
        };
    }
}