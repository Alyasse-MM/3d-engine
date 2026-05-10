#pragma once
#include <SFML/Graphics.hpp>
#include "Maths/Vector3.hpp"
#include "Maths/Utils.hpp"

namespace al3d {
    namespace Rendering {
        struct EngineConfig {
            al3d::Maths::Vector3<float> lightDirections = { 0,0,-1 };
            float minDarkness = 0.f;
            unsigned windowWidth = 1920;
            unsigned windowHeight = 1080;
            float fov = 90.0f;
            float nearClipPlane = 0.1f;
            unsigned frameRate = 60;
            float focalLength;
            float halfWidth;
            float halfHeight;

            EngineConfig();

            void updateCalculatedValues();
        };
    }
}