#pragma once

#include <Job.h>
#include <Cube.h>
#include <Point.h>

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
