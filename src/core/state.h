#pragma once
#include "maths/Vector3.h"

struct EngineState {
    unsigned windowWidth = 1920;
    unsigned windowHeight = 1080;

    unsigned antialiasinglevel = 20;

    float fov = 90.0f;

    Maths::Vector3<float> camera_pos = { 0.0f, 0.0f, -4.0f };
    float camera_yaw = 0.0f;
    float camera_pitch = 0.0f;
    float camera_roll = 0.0f;

    float model_angle_x = 0.0f;
    float model_angle_y = 0.0f;

    float z_near = 0.1f;
};