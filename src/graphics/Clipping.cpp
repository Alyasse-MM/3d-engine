#include "Clipping.h"
#include <cmath>

namespace Graphics {
    std::vector<Vector3<float>> clipPolygon(const std::vector<Vector3<float>>& polygon) {
        std::vector<Vector3<float>> outPoly;
        float z_near = 0.1f;

        if (polygon.empty()) return outPoly;

        Vector3<float> prev = polygon.back();
        bool prevInside = (prev.z >= z_near);

        for (const auto& curr : polygon) {
            bool currInside = (curr.z >= z_near);

            if (currInside && prevInside) {
                outPoly.push_back(curr);
            }
            else if (!currInside && prevInside) {
                float t = (z_near - prev.z) / (curr.z - prev.z);
                Vector3<float> intersect = prev + (curr - prev) * t;
                outPoly.push_back(intersect);
            }
            else if (currInside && !prevInside) {
                float t = (z_near - prev.z) / (curr.z - prev.z);
                Vector3<float> intersect = prev + (curr - prev) * t;
                outPoly.push_back(intersect);
                outPoly.push_back(curr);
            }

            prev = curr;
            prevInside = currInside;
        }

        return outPoly;
    }
}