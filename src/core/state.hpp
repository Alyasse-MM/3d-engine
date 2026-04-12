#pragma once
#include "maths/Vector3.hpp"

namespace al3d
{
    struct EngineState {
        unsigned windowWidth = 1920;
        unsigned windowHeight = 1080;

        unsigned antiAliasingLevel = 0;

        float fov = 90.0f;

        Maths::Vector3<float> cameraPosition = { 0.0f, 0.0f, -4.0f };
        float cameraYaw = 0.0f;
        float cameraPitch = 0.0f;
        float cameraRoll = 0.0f;

        float modelAngleX = 0.0f;
        float modelAngleY = 0.0f;

        float nearClipPlane = 0.1f;
    };
}