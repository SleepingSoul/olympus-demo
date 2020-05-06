#pragma once

#include <Job.h>
#include <DrawCall.h>

BeginNamespaceOlympus

class OpenGLVoxelRenderer;
class OpenGLGLFWContext;

class RenderFrameJob : public Job
{
public:
    using Base = Job;

    struct InitParameters
    {
        std::shared_ptr<OpenGLGLFWContext> context;
        std::shared_ptr<OpenGLVoxelRenderer> renderer;
        std::vector<VoxelDrawCall> drawCalls;
    };

    RenderFrameJob(InitParameters&& initParams);

    std::future<void> getRenderFinishedFuture();

    void execute() override;

private:
    InitParameters m_params;
    std::promise<void> m_renderFinished;
};

EndNamespaceOlympus
