#pragma once
#include "softwareRenderer.h"
#include "maths/Math.h"
#include "graphics/Clipping.h"
#include <algorithm>

using namespace Maths;

inline bool SoftwareRenderer::backfaceCulling(const std::vector<Vector3<float>>& faceVerts) {
    if (faceVerts.size() >= 3) {
        Vector3<float> normal = cross(faceVerts[1] - faceVerts[0], faceVerts[2] - faceVerts[0]);
        return (dot(normal, faceVerts[0]) >= 0);
    }
    return false;
}