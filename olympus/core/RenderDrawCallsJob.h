#pragma once

#include <Job.h>
#include <DrawCall.h>

BeginNamespaceOlympus

class OpenGLVoxelRenderer;
class OpenGLGLFWContext;

class RenderDrawCallsJob : public Job
{
public:
    using Base = Job;

    struct InitParameters
    {
        std::shared_ptr<OpenGLGLFWContext> context;
        std::shared_ptr<OpenGLVoxelRenderer> renderer;
        std::vector<VoxelDrawCall> drawCalls;
    };

    RenderDrawCallsJob(InitParameters&& initParams);

    std::future<void> getRenderFinishedFuture();

    void execute() override;

private:
    InitParameters m_params;
    std::promise<void> m_renderFinished;
};

EndNamespaceOlympus
