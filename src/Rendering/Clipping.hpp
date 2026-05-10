#pragma once
#include <vector>
#include "Maths/Vector3.hpp"

namespace al3d
{
    namespace Rendering {
        std::vector<Maths::Vector3<float>> sutherlandHodgmanZ(const Maths::Vector3<float> polygon[3], float nearClipPlane);
        std::vector<std::vector<Maths::Vector3<float>>> clipPolygon(const Maths::Vector3<float> polygon[3], float nearClipPlane);
    }
}