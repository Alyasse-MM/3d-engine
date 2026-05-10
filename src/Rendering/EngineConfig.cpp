#pragma once
#include "EngineConfig.hpp"
#include "Maths/Utils.hpp"

namespace al3d {
    namespace Rendering {
        EngineConfig::EngineConfig() {
            updateCalculatedValues();
        }

        void EngineConfig::updateCalculatedValues() {
            focalLength = Maths::calculateFocalLength(fov, windowWidth);
            halfWidth = windowWidth / 2.0f;
            halfHeight = windowHeight / 2.0f;
        }
    }
}