#pragma once

#include <opencv2/opencv.hpp>

#include <OpenGLGLFWContext.h>
#include <OpenGLRenderer.h>
#include <TextureStorage.h>
#include <JobSystem.h>
#include <AsyncVideostreamListener.h>

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

    double getTimeFromStart() const;

    int run();

private:
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

    std::atomic_bool m_isRecognizing{ false };

    std::vector<cv::Vec2i> m_recognized;
};

EndNamespaceOlympus
