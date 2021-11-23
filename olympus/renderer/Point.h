#pragma once

#include <glm/glm.hpp>

#include <opencv2/core/mat.hpp>

#include <utils/macros.h>

BeginNamespaceOlympus

struct Point2D
{
    glm::vec4 color{ 1.f, 1.f, 1.f, 1.f };
    glm::vec3 position{ 0.f, 0.f, 0.f };
    cv::Mat viewmodel;
    float size{ 5.f };
};

EndNamespaceOlympus
