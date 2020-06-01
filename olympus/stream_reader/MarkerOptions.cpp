#include "MarkerOptions.h"

BeginNamespaceOlympus

namespace
{
    const char* const CameraMatrixNodeName = "mtx";
    const char* const DistortionMatrixName = "dist";
}

namespace markers
{
    MarkerRecognitionOptions::MarkerRecognitionOptions(const cv::String& pathToOptions, const cv::Size& frameSize)
        : m_frameSize(frameSize)
    {
        cv::FileStorage storage(pathToOptions, cv::FileStorage::READ);

        auto cameraMatrix = storage[CameraMatrixNodeName].mat();
        auto distortionMatrix = storage[DistortionMatrixName].mat();

        m_parameters.setParams(cameraMatrix, distortionMatrix, frameSize);

        m_detector.setDetectionMode(aruco::DetectionMode::DM_FAST);
    }
}

EndNamespaceOlympus