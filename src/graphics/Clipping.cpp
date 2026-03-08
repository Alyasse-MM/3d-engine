#include "graphics/Clipping.h"
#include <cmath>
#include <maths/Utils.h>

using namespace Maths;

namespace Graphics {

    std::vector<Vector3<float>> clipPolygon(const std::vector<Vector3<float>>& polygon, float z_near) {
        if (polygon.empty()) return {};

        std::vector<Vector3<float>> outPoly;
        

        // Sutherland-Hodgman Algorithm
        Vector3<float> prev = polygon.back();
        bool prevInside = (prev.z >= z_near);

        for (const auto& curr : polygon) {
            bool currInside = (curr.z >= z_near);

            if (currInside) {
                if (!prevInside) {
                    // Entering the visible area: add intersection point first
                    outPoly.push_back(intersectZNear(prev, curr, z_near));
                }
                outPoly.push_back(curr);
            }
            else if (prevInside) {
                // Leaving the visible area: add intersection point
                outPoly.push_back(intersectZNear(prev, curr, z_near));
            }

            prev = curr;
            prevInside = currInside;
        }

        return outPoly;
    }
}