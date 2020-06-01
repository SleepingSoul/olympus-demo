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

        auto getNearDistance() const { return m_nearDistance; }
        auto getFarDistance() const { return m_farDistance; }

        void setNearDistance(double nearDistance) { m_nearDistance = nearDistance; }
        void setFarDistance(double farDistance) { m_farDistance = farDistance; }

        const auto& getFrameSize() const { return m_frameSize; }

    private:
        aruco::MarkerDetector m_detector;
        aruco::CameraParameters m_parameters;
        cv::Size m_frameSize;
        double m_nearDistance{ 0.1 };
        double m_farDistance{ 100. };
    };
}

EndNamespaceOlympus
