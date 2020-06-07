#include "marker_detector.h"

#include <array>
#include <optional>

#include <easy/profiler.h>

#include <aruco.h>

#include <logging/logging.h>
#include <utils/olyerror.h>

BeginNamespaceOlympus

namespace markers
{
    std::vector<DetectResult> detectMarkers(MarkerRecognitionOptions& options, cv::Mat frame)
    {
        EASY_FUNCTION(profiler::colors::DarkGreen);

        std::vector<DetectResult> result;
        std::vector<aruco::Marker> markers;

        try
        {
            options.getDetector().detect(frame, markers, options.getParameters(), 0.178f);
        }
        catch (std::exception& e)
        {
            olyError("[marker_detector] Exception on detect: {}", e.what());
            return result;
        }

        for (auto& marker : markers)
        {
            EASY_BLOCK("Marker gl transformations", profiler::colors::Green900);

            if (!marker.isPoseValid())
            {
                logging::info("[marker_detector] Pose not valid.");
                continue;
            }

            cv::Mat glModelView(4, 4, CV_64F);
            marker.glGetModelViewMatrix(&glModelView.at<double>(0));
            glModelView.convertTo(glModelView, CV_32F);

            cv::Mat glProjection(4, 4, CV_64F);
            options.getParameters().glGetProjectionMatrix(frame.size(), frame.size(), &glProjection.at<double>(0),
                options.getNearDistance(), options.getFarDistance());
            glProjection.convertTo(glProjection, CV_32F);

            result.emplace_back(DetectResult{ marker.id, glModelView, glProjection });
        }

        return result;
    }
}

EndNamespaceOlympus