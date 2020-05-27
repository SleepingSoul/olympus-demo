#include "Recognizer.h"

#include <easy/profiler.h>

#include <utils/olyerror.h>
#include <utils/math.h>

BeginNamespaceOlympus

namespace recognizer
{
    std::vector<cv::Vec2i> recognizeBase(const RecognitionOptions& options, const cv::Mat& frame)
    {
        EASY_FUNCTION(profiler::colors::Green100);

        if (frame.empty())
        {
            return {};
        }

        if (options.numContours == 0 || options.contourApproxMode == cv::ContourApproximationModes::CHAIN_APPROX_NONE)
        {
            olyError("[Recognizer] Invalid recognition options given.");
            return {};
        }

        cv::Mat threshold{};

        EASY_BLOCK("BGR2HSV conversion", profiler::colors::Green200);
        cv::cvtColor(frame, threshold, cv::COLOR_BGR2HSV);
        EASY_END_BLOCK;

        if (options.preprocessingBlurRadius)
        {
            EASY_BLOCK("Preprocessing blur", profiler::colors::Green200);
            cv::medianBlur(threshold, threshold, options.preprocessingBlurRadius);
        }

        cv::Mat lowerRedFilter{}, upperRedFilter{};

        EASY_BLOCK("Lower and upper values filtering", profiler::colors::Green200);
        cv::inRange(threshold, options.lowerHSVBegin, options.lowerHSVEnd, lowerRedFilter);
        cv::inRange(threshold, options.upperHSVBegin, options.upperHSVEnd, upperRedFilter);
        EASY_END_BLOCK;

        EASY_BLOCK("Weightening", profiler::colors::Green200);
        cv::addWeighted(lowerRedFilter, 1., upperRedFilter, 1., 0., threshold);
        EASY_END_BLOCK;

        std::vector<cv::Mat> contours;
        contours.reserve(options.numContours * 5ull);

        EASY_BLOCK("Find contours", profiler::colors::Green200);
        cv::findContours(threshold, contours, options.contourRetrievalMode, options.contourApproxMode);
        EASY_END_BLOCK;

        EASY_BLOCK("Removing zero contours", profiler::colors::Green200);
        const auto newEnd = std::remove_if(contours.begin(), contours.end(), [](const cv::Mat& contour)
        {
            return equalsWithAlpha(cv::contourArea(contour), 0., 0.001);
        });

        contours.erase(newEnd, contours.end());
        EASY_END_BLOCK;

        EASY_BLOCK("Sort contours by area", profiler::colors::Green200);
        std::sort(contours.begin(), contours.end(), [](const auto& left, const auto& right)
        {
            return cv::contourArea(left) > cv::contourArea(right);
        });
        EASY_END_BLOCK;

        if (contours.size() > options.numContours)
        {
            contours.resize(options.numContours);
        }

        std::vector<cv::Vec2i> result;
        result.reserve(options.numContours);

        std::transform(contours.cbegin(), contours.cend(), std::back_inserter(result), [](const cv::Mat& contour)
        {
            const cv::Moments moment = cv::moments(contour);
            return cv::Vec2i( static_cast<int>(std::round(moment.m10 / moment.m00)),
                static_cast<int>(std::round(moment.m01 / moment.m00 )));
        });

        return result;
    }

}

EndNamespaceOlympus
