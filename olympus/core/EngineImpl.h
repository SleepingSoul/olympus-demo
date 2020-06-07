#pragma once

#include <opencv2/opencv.hpp>

#include <OpenGLGLFWContext.h>
#include <OpenGLRenderer.h>
#include <marker_detector.h>
#include <TextureStorage.h>
#include <JobSystem.h>
#include <AsyncVideostreamListener.h>
#include <EngineSubsystem.h>

BeginNamespaceOlympus

class EngineImpl
{
    OlyNonCopyableMovable(EngineImpl)
public:
    EngineImpl() = default;

    void initialize();

    bool initializedSuccessfuly() const { return m_successfulInitialization; }

    AsyncVideostreamListener& getAsyncVideostreamListener() { return m_listener; }
    JobSystem& getJobSystem() { return m_jobSystem; }
    OpenGLGLFWContext& getWindowContext() { return *m_openGLGLFWContext; }
    OpenGLRenderer& getRenderer() { return *m_openGLRenderer; }
    TextureStogare& getTextureStorage() { return m_texStorage; }

    bool isDebugMode() const { return m_isDebugMode; }

    double getTimeFromStart() const;

    void setMarkers(std::vector<markers::DetectResult>&& markers) { m_markers = std::move(markers); }
    const auto& getMarkers() const { return m_markers; }

    int run();

private:
    std::vector<std::unique_ptr<EngineSubsystem>> m_subsystems;

    std::vector<markers::DetectResult> m_markers;

    void registerCallbacksAndDebugText();

    void initGLFWContext();
    [[nodiscard]] std::future<void> prepeareAndSendRenderFrameJob();

    std::optional<std::string> m_profilerFile;

    std::shared_ptr<OpenGLGLFWContext> m_openGLGLFWContext;
    std::shared_ptr<OpenGLRenderer> m_openGLRenderer;

    JobSystem m_jobSystem;

    AsyncVideostreamListener m_listener;
    AsyncVideostreamListener::FrameID m_lastFrameID{ 0 };

    TextureStogare m_texStorage;

    bool m_successfulInitialization{ false };

    bool m_isDebugMode{ false };
};

EndNamespaceOlympus
