#include "graphics/Clipping.hpp"
#include <cmath>
#include <maths/Utils.hpp>

using namespace Maths;

namespace Graphics {
    std::vector<Vector3<float>> sutherlandHodgmanZ(const std::vector<Vector3<float>>& polygon, float z_near) {
        if (polygon.empty()) return {};

        std::vector<Vector3<float>> poly;
        Vector3<float> prev = polygon.back();
        bool prevInside = (prev.z >= z_near);

        for (const auto& curr : polygon) {
            bool currInside = (curr.z >= z_near);

            float t = (z_near - prev.z) / (curr.z - prev.z);
            if (currInside) {
                if (!prevInside) {
                    poly.push_back(Maths::lerp(prev, curr, t));
                }
                poly.push_back(curr);
            }
            else if (prevInside) {
                poly.push_back(Maths::lerp(prev, curr, t));
            }

            prev = curr;
            prevInside = currInside;
        }
        return poly;
    }

    std::vector<std::vector<Vector3<float>>> clipPolygon(const std::vector<Vector3<float>>& polygon, float z_near) {
        std::vector<Vector3<float>> poly{ sutherlandHodgmanZ(polygon, z_near) };
        if (poly.size() < 3) {
            return {};
        }
        std::vector<std::vector<Vector3<float>>> clipped;

        for (size_t i = 1; i < poly.size() - 1; ++i) {
            std::vector<Vector3<float>> tri;
            tri.push_back(poly[0]);
            tri.push_back(poly[i]);
            tri.push_back(poly[i + 1]);
            clipped.push_back(tri);
        }
        return clipped;
    }
}