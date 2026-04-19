#pragma once

#include "Maths/Vector3.hpp"

namespace al3d {
    namespace Core {
        struct Vertex {
            Maths::Vector3<float> position;
            Maths::Vector3<float> normal;
        };
    }
}