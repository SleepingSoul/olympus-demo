#include <pch.h>
#include "RenderFrameJob.h"
#include <Engine.h>
#include <AsyncVideostreamListener.h>
#include <JobAffinity.h>
#include <OpenGLGLFWContext.h>
#include <OpenGLRenderer.h>
#include <Texture.h>

BeginNamespaceOlympus

RenderFrameJob::RenderFrameJob(InitParameters&& initParams)
    : Base("Render draw calls", JobAffinity::Render, /*silent*/true)
    , m_params(std::move(initParams))
{}

std::future<void> RenderFrameJob::getRenderFinishedFuture()
{
    return m_renderFinished.get_future();
}

void RenderFrameJob::execute()
{
    EASY_FUNCTION(profiler::colors::Red);

    m_params.context->renderFrameStart();

    auto& listener = olyEngine.getAsyncVideostreamListener();

    if (m_params.backgroundUpdated && listener.isListening())
    {
        const auto lastFrame = listener.getLatestFrame();

        const auto& camera = m_params.renderer->getCamera();

        EASY_BLOCK("Undistort + flip", profiler::colors::Red50);

        const auto cameraMatrix = camera.getCameraMatrix();
        const auto distortionMatrix = camera.getDistortionMatrix();

        cv::Mat distorted = lastFrame.clone();

        if (!cameraMatrix.empty() && !distortionMatrix.empty())
        {
            cv::undistort(lastFrame, distorted, cameraMatrix, distortionMatrix);
        }

        cv::flip(distorted, distorted, 0);

        EASY_END_BLOCK;

        if (!distorted.empty())
        {
            const auto size = distorted.size();
            EASY_BLOCK("Resetting the texture", profiler::colors::DarkRed);
            m_params.backgroundTexture->hotReset(distorted.ptr(), size.width, size.height, GL_BGR);
            EASY_END_BLOCK;
        }
    }

    m_params.renderer->getBackgroundRenderComponent().setBackgroundTexture(m_params.backgroundTexture);

    m_params.renderer->render();

    m_params.context->renderFrameEnd();

    m_renderFinished.set_value();
}

EndNamespaceOlympus
