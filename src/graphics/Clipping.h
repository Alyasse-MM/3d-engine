#pragma once
#include <vector>
#include "maths/Vector3.h"

namespace Graphics {
    std::vector<Vector3<float>> clipPolygon(const std::vector<Vector3<float>>& polygon);
}