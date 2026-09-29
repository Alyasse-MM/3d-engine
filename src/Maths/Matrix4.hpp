#pragma once

namespace al3d
{
    namespace Maths {
        template <typename T>
        struct Matrix4 {
            T data[16] = {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            };

            static Matrix4 getTranslation(const Vector3<T>& pos) {
                return Matrix4{ {
                    1, 0, 0, pos.x,
                    0, 1, 0, pos.y,
                    0, 0, 1, pos.z,
                    0, 0, 0, 1
                } };
            }

            static Matrix4 getHomothety(T k) {
                return Matrix4{ {
                    k, 0, 0, 0,
                    0, k, 0, 0,
                    0, 0, k, 0,
                    0, 0, 0, 1
                } };
            }

            static Matrix4 getRotationX(T theta) {
                T c = std::cos(theta);
                T s = std::sin(theta);
                return Matrix4{ {
                    1, 0,  0, 0,
                    0, c, -s, 0,
                    0, s,  c, 0,
                    0, 0,  0, 1
                } };
            }

            static Matrix4 getRotationY(T theta) {
                T c = std::cos(theta);
                T s = std::sin(theta);
                return Matrix4{ {
                     c, 0, s, 0,
                     0, 1, 0, 0,
                    -s, 0, c, 0,
                     0, 0, 0, 1
                } };
            }

            static Matrix4 getRotationZ(T theta) {
                T c = std::cos(theta);
                T s = std::sin(theta);
                return Matrix4{ {
                    c, -s, 0, 0,
                    s,  c, 0, 0,
                    0,  0, 1, 0,
                    0,  0, 0, 1
                } };
            }
        };
    }
}