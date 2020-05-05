#pragma once

#include <Job.h>

BeginNamespaceOlympus

class OpenGLGLFWContext;
class OpenGLVoxelRenderer;

// OpenGL libraries use global state - so it should be initialized in the render thread.
class InitializeRenderJob : public Job
{
public:
    using Base = Job;

    struct InitParameters
    {
        std::shared_ptr<OpenGLGLFWContext>& outContext;
        std::shared_ptr<OpenGLVoxelRenderer>& outRenderer;
    };

    InitializeRenderJob(InitParameters initParams);

    std::future<bool> getSuccessFuture();
    void execute() override;

private:
    InitParameters m_initialization;
    std::promise<bool> m_initSuccess;
};

EndNamespaceOlympus
