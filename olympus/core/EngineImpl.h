#pragma once

#include <OpenGLGLFWContext.h>
#include <OpenGLVoxelRenderer.h>
#include <JobSystem.h>

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

    std::optional<std::string> m_profilerFile;

    std::shared_ptr<OpenGLGLFWContext> m_openGLGLFWContext;
    std::shared_ptr<OpenGLVoxelRenderer> m_openGLVoxelRenderer;
    JobSystem m_jobSystem;

    bool m_successfulInitialization{ false };
};

EndNamespaceOlympus
