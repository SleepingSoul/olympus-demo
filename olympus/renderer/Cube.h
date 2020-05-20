#pragma once

#include <glm/glm.hpp>

#include <utils/macros.h>

BeginNamespaceOlympus

class Texture;

struct Cube
{
    glm::vec3 position{ 0.f, 0.f, 0.f };
    
    glm::vec3 rotationAxis{ 1.f, 0.f, 0.f };
    float rotationValueRad{ 0.f };

    float edgeSize{ 1.f };

    Texture* face{ nullptr };
};

EndNamespaceOlympus
