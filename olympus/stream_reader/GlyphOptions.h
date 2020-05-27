#pragma once

#include <array>

#include <opencv2/opencv.hpp>

#include <utils/macros.h>

BeginNamespaceOlympus

namespace glyph
{

    class GlyphRecognitionOptions
    {
        OlyNonCopyable(GlyphRecognitionOptions)
    public:
        GlyphRecognitionOptions(const cv::String& pathToOptions);

        const auto& getCameraMatrix() const { return m_cameraMatrix; }
        const auto& getDistortionMatrix() const { return m_distortionMatrix; }
        const auto& getSize() const { return m_size; }
        const auto& getObjPoints() const { return m_objectPoints; }

    private:
        cv::Mat m_cameraMatrix;
        cv::Mat m_distortionMatrix;
        cv::Mat m_objectPoints;
        cv::Size m_size;
    };

}

EndNamespaceOlympus
