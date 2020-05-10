#pragma once

#include <OpenGLGLFWContext.h>
#include <OpenGLVoxelRenderer.h>
#include <JobSystem.h>
#include <AsyncVideostreamListener.h>

BeginNamespaceOlympus

class EngineImpl
{
    OlyNonCopyableMovable(EngineImpl)
public:
    EngineImpl();

    bool initializedSuccessfuly() const { return m_successfulInitialization; }

    int run();

private:
    void initGLFWContext();
    [[nodiscard]] std::future<void> prepeareAndSendRenderFrameJob();

    std::optional<std::string> m_profilerFile;

    std::shared_ptr<OpenGLGLFWContext> m_openGLGLFWContext;
    std::shared_ptr<OpenGLVoxelRenderer> m_openGLVoxelRenderer;

    AsyncVideostreamListener m_listener;

    JobSystem m_jobSystem;
    StreamReader m_streamReader;


    bool m_successfulInitialization{ false };
};

EndNamespaceOlympus
