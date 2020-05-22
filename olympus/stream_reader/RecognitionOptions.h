#pragma once

#include <opencv2/opencv.hpp>

#include <utils/macros.h>

BeginNamespaceOlympus

namespace recognizer
{

    struct RecognitionOptions
    {
        unsigned preprocessingBlurRadius{ 0 };

        unsigned numContours{ 0 };

        cv::Vec3i lowerHSVBegin{ 0,0,0 };
        cv::Vec3i lowerHSVEnd{ 0,0,0 };
        cv::Vec3i upperHSVBegin{ 0,0,0 };
        cv::Vec3i upperHSVEnd{ 0,0,0 };

        cv::RetrievalModes contourRetrievalMode{ cv::RetrievalModes::RETR_CCOMP };
        cv::ContourApproximationModes contourApproxMode{ cv::ContourApproximationModes::CHAIN_APPROX_NONE };
    };

}

EndNamespaceOlympus
