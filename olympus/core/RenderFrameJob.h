#pragma once

#include <Job.h>
#include <DrawCall.h>

BeginNamespaceOlympus

class OpenGLRenderer;
class OpenGLGLFWContext;

class RenderFrameJob : public Job
{
public:
    using Base = Job;

    struct InitParameters
    {
        std::shared_ptr<OpenGLGLFWContext> context;
        std::shared_ptr<OpenGLRenderer> renderer;
        std::vector<VoxelDrawCall> drawCalls;
        Texture* backgroundTexture;
        bool backgroundUpdated;
    };

    RenderFrameJob(InitParameters&& initParams);

    std::future<void> getRenderFinishedFuture();

    void execute() override;

private:
    InitParameters m_params;
    std::promise<void> m_renderFinished;
};

EndNamespaceOlympus
