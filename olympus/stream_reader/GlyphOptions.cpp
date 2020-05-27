#include "GlyphOptions.h"

BeginNamespaceOlympus

namespace
{
    const char* const CameraMatrixNodeName = "mtx";
    const char* const DistortionMatrixName = "dist";
    const char* const ObjPointsMatrixName = "objp";
}

namespace glyph
{
    GlyphRecognitionOptions::GlyphRecognitionOptions(const cv::String& pathToOptions)
    {
        cv::FileStorage storage(pathToOptions, cv::FileStorage::READ);

        m_cameraMatrix = storage[CameraMatrixNodeName].mat();
        m_distortionMatrix = storage[DistortionMatrixName].mat();
        
        m_size.width = 9;
        m_size.height = 6;

        m_objectPoints = storage[ObjPointsMatrixName].mat();
    }
}

EndNamespaceOlympus