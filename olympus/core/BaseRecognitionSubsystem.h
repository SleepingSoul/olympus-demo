#pragma once

#include <glm/gtc/matrix_transform.hpp>

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
    std::mutex m_mutex;
    AsyncVideostreamListener::FrameID m_lastFrameID{ 0 };
    glm::mat4 m_viewMatrix{ glm::identity<glm::mat4>() };
    glyph::GlyphRecognitionOptions m_glyphOptions;
    std::atomic_bool m_isRecognizing{ false };
};

EndNamespaceOlympus
