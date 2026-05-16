#pragma once
#include "Maths/Vector3.hpp"

namespace al3d
{
    namespace Rendering {
        struct CameraState {
            Maths::Vector3<float> cameraPosition = { 0.0f, 0.0f, -4.0f };
            float cameraYaw = 0.0f;
            float cameraPitch = 0.0f;
            float cameraRoll = 0.0f;

            float modelAngleX = 0.0f;
            float modelAngleY = 0.0f;
        };
    }
}