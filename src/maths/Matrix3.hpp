#pragma once

namespace al3d
{
    namespace Maths {
        template <typename T>
        struct Matrix3 {
            T data[9] = { 0 };

            T& operator()(int row, int col) { return data[row * 3 + col]; }

            static Matrix3 getRotationX(T theta) {
                T c = std::cos(theta);
                T s = std::sin(theta);
                return Matrix3{ { 1, 0, 0, 0, c, -s, 0, s, c} };
            }

            static Matrix3 getRotationY(T theta) {
                T c = std::cos(theta);
                T s = std::sin(theta);
                return Matrix3{ { c, 0, s, 0, 1, 0, -s, 0, c } };
            }
            
            static Matrix3 getRotationZ(T theta) {
                T c = std::cos(theta);
                T s = std::sin(theta);
                return Matrix3{ { c, -s, 0, s, c, 0, 0, 0, 1 } };
            }

            static Matrix3 getHomothety(T k) {
                return Matrix3{ {
                    k, 0, 0,
                    0, k, 0,
                    0, 0, k
                } };
            }
        };
    }
}