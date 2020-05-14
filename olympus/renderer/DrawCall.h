#pragma once

#include <utils/macros.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

BeginNamespaceOlympus

class Texture;

class DrawCall
{
    Texture* texture;
    glm::vec4 colorMask{ 1.f, 1.f, 1.f, 1.f };
    glm::vec3 position;
    glm::vec3 rotationVec;
    float andle;
    float width;
    float height;
};

struct VoxelDrawCall
{
    Texture* texture;
    glm::vec4 colorMask;
    glm::vec3 position;
    glm::vec3 rotationVec;
    float angle;
};

EndNamespaceOlympus
