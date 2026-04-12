#include "graphics/Clipping.hpp"
#include <cmath>
#include <maths/Utils.hpp>

namespace al3d
{
    using namespace Maths;

    namespace Graphics {
        std::vector<Vector3<float>> sutherlandHodgmanZ(const std::vector<Vector3<float>>& polygon, float nearClipPlane) {
            if (polygon.empty()) return {};

            std::vector<Vector3<float>> clippedVertices;
            Vector3<float> edgeStart = polygon.back();
            bool edgeStartInside = (edgeStart.z >= nearClipPlane);

            for (const auto& edgeEnd : polygon) {
                bool edgeEndInside = (edgeEnd.z >= nearClipPlane);

                float intersectionRatio = (nearClipPlane - edgeStart.z) / (edgeEnd.z - edgeStart.z);
                if (edgeEndInside) {
                    if (!edgeStartInside) {
                        clippedVertices.push_back(Maths::lerp(edgeStart, edgeEnd, intersectionRatio));
                    }
                    clippedVertices.push_back(edgeEnd);
                }
                else if (edgeStartInside) {
                    clippedVertices.push_back(Maths::lerp(edgeStart, edgeEnd, intersectionRatio));
                }

                edgeStart = edgeEnd;
                edgeStartInside = edgeEndInside;
            }
            return clippedVertices;
        }

        std::vector<std::vector<Vector3<float>>> clipPolygon(const std::vector<Vector3<float>>& polygon, float nearClipPlane) {
            std::vector<Vector3<float>> poly{ sutherlandHodgmanZ(polygon, nearClipPlane) };
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
}