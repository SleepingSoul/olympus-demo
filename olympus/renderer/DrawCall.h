#pragma once

#include <utils/macros.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

BeginNamespaceOlympus

class Texture;

struct VoxelDrawCall
{
    Texture* texture;
    glm::vec4 colorMask;
    glm::vec3 position;
    glm::vec3 rotationVec;
    float angle;
};

EndNamespaceOlympus
