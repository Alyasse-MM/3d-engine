#pragma once
#include <cmath>
#include <vector>

struct Vec3 {
    float x, y, z;

    Vec3 operator+(const Vec3& other) const { return {x + other.x, y + other.y, z + other.z}; }
    Vec3 operator-(const Vec3& other) const { return {x - other.x, y - other.y, z - other.z}; }
    Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
};

struct Matrix3 {
    float m[3][3];
};

class Maths {
public:
    static float toRadians(float degrees) {
        return degrees * 3.14159f / 180.0f;
    }

    static Vec3 matMul(const Matrix3& mat, const Vec3& v) {
        return {
            mat.m[0][0]*v.x + mat.m[0][1]*v.y + mat.m[0][2]*v.z,
            mat.m[1][0]*v.x + mat.m[1][1]*v.y + mat.m[1][2]*v.z,
            mat.m[2][0]*v.x + mat.m[2][1]*v.y + mat.m[2][2]*v.z
        };
    }

    // Cross Product For calculating normals
    static Vec3 crossProduct(Vec3 a, Vec3 b) {
        return {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }

    static Matrix3 getRotationX(float theta) {
        float c = cos(theta);
        float s = sin(theta);
        return Matrix3 {{ {1, 0, 0}, {0, c, -s}, {0, s, c} }};
    }

    static Matrix3 getRotationY(float theta) {
        float c = cos(theta);
        float s = sin(theta);
        return Matrix3 {{ {c, 0, s}, {0, 1, 0}, {-s, 0, c} }};
    }

    static std::vector<Vec3> clipPolygon(const std::vector<Vec3>& polygon) {
        std::vector<Vec3> outPoly;
        float z_near = 0.1f;

        if (polygon.empty()) return outPoly;

        Vec3 prev = polygon.back();
        bool prevInside = (prev.z >= z_near);

        for (const auto& curr : polygon) {
            bool currInside = (curr.z >= z_near);

            if (currInside && prevInside) {
                outPoly.push_back(curr);
            }
            else if (!currInside && prevInside) {
                float t = (z_near - prev.z) / (curr.z - prev.z);
                Vec3 intersect = prev + (curr - prev) * t; 
                outPoly.push_back(intersect);
            }
            else if (currInside && !prevInside) {
                float t = (z_near - prev.z) / (curr.z - prev.z);
                Vec3 intersect = prev + (curr - prev) * t;
                outPoly.push_back(intersect);
                outPoly.push_back(curr);
            }

            prev = curr;
            prevInside = currInside;
        }

        return outPoly;
    }
};