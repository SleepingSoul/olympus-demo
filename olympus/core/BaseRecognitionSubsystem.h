#pragma once

#include <marker_detector.h>

#include <EngineSubsystem.h>
#include <AsyncVideostreamListener.h>
#include <RecognizeBaseJob.h>

BeginNamespaceOlympus

class BaseRecognitionSubsystem : public EngineSubsystem
{
public:
    using Base = EngineSubsystem;

    BaseRecognitionSubsystem(EngineImpl& engine);

    void update() override;

private:
    AsyncVideostreamListener::FrameID m_lastFrameID{ 0 };

    std::mutex m_mutex;
    std::vector<markers::DetectResult> m_detectResult;
    bool m_detected{ false };

    markers::MarkerRecognitionOptions m_markerOptions;

    std::atomic_bool m_isRecognizing{ false };

    boost::circular_buffer<unsigned> m_latestFPS;

    std::atomic<unsigned> m_normalizedFPS{ 0u };

    double m_lastTimestamp{ 0. };
};

EndNamespaceOlympus
