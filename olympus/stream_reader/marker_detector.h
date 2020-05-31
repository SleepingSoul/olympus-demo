#pragma once

#include <optional>

#include <utils/macros.h>

#include <MarkerOptions.h>

BeginNamespaceOlympus

namespace markers
{
    struct DetectResult
    {
        cv::Mat modelviewMatrix;
        cv::Mat projectionMatrix;
    };

    std::optional<DetectResult> detectMarker(MarkerRecognitionOptions& options, cv::Mat frame);
}

EndNamespaceOlympus
