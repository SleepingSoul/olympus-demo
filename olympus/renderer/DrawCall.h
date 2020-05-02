#pragma once

#include <utils/macros.h>

BeginNamespaceOlympus

struct VoxelDrawCall
{
    glm::vec4 color;
    glm::vec3 position;
    glm::vec3 rotationVec;
    float angle;
};

EndNamespaceOlympus
