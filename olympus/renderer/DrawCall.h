#pragma once

#include <utils/macros.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

BeginNamespaceOlympus

struct VoxelDrawCall
{
    glm::vec4 color;
    glm::vec3 position;
    glm::vec3 rotationVec;
    float angle;
};

EndNamespaceOlympus
