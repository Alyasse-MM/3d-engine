#include "graphics/Clipping.hpp"
#include <cmath>
#include <maths/Utils.hpp>

using namespace Maths;

namespace Graphics {
    inline std::vector<Vector3<float>> sutherlandHodgmanZ(const std::vector<Vector3<float>>& polygon, float z_near) {
        if (polygon.empty()) return {};

        std::vector<Vector3<float>> outPoly;
        Vector3<float> prev = polygon.back();
        bool prevInside = (prev.z >= z_near);

        for (const auto& curr : polygon) {
            bool currInside = (curr.z >= z_near);

            float t = (z_near - prev.z) / (curr.z - prev.z);
            if (currInside) {
                if (!prevInside) {
                    outPoly.push_back(Maths::lerp(prev, curr, t));
                }
                outPoly.push_back(curr);
            }
            else if (prevInside) {
                outPoly.push_back(Maths::lerp(prev, curr, t));
            }

            prev = curr;
            prevInside = currInside;
        }
        return outPoly;
    }

    std::vector<Vector3<float>> clipPolygon(const std::vector<Vector3<float>>& polygon, float z_near) {
        return sutherlandHodgmanZ(polygon, z_near);
    }
}