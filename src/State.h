#pragma once
#include "Maths.h"

struct EngineState {
    float angle_x = 0.0f;
    float angle_y = 0.0f;
    
    Vec3 camera_pos = {0.0f, 0.0f, -4.0f};
    float camera_yaw = 0.0f;
};