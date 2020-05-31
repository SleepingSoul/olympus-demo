#pragma once

#include <array>

#include <opencv2/opencv.hpp>

#include <aruco.h>

#include <utils/macros.h>

BeginNamespaceOlympus

namespace markers
{

    class MarkerRecognitionOptions
    {
        OlyNonCopyable(MarkerRecognitionOptions)
    public:
        MarkerRecognitionOptions(const cv::String& pathToCalibrationData, const cv::Size& frameSize);

        auto& getDetector() { return m_detector; }
        auto& getParameters() { return m_parameters; }

    private:
        aruco::MarkerDetector m_detector;
        aruco::CameraParameters m_parameters;
    };

}

EndNamespaceOlympus
