#pragma once

#include <vector>

#include <utils/macros.h>

#include <MarkerOptions.h>

BeginNamespaceOlympus

namespace markers
{
    struct DetectResult
    {
        using MarkerID = int;

        MarkerID markerID;
        cv::Mat modelviewMatrix;
        cv::Mat projectionMatrix;
    };

    std::vector<DetectResult> detectMarkers(MarkerRecognitionOptions& options, cv::Mat frame);
}

EndNamespaceOlympus
