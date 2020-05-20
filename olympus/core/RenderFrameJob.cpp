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
    EASY_FUNCTION("Num draw calls = %zu", m_params.cubes.size(), profiler::colors::Red);

    m_params.context->renderFrameStart();

    auto& listener = olyEngine.getAsyncVideostreamListener();

    if (m_params.backgroundUpdated && listener.isListening())
    {
        const auto lastFrame = listener.getLatestFrame();

        if (!lastFrame.empty())
        {
            const auto size = lastFrame.size();
            EASY_BLOCK("Resetting the texture", profiler::colors::DarkRed);
            m_params.backgroundTexture->hotReset(lastFrame.ptr(), size.width, size.height, GL_BGR);
            EASY_END_BLOCK;
        }
    }

    m_params.renderer->getBackgroundRenderComponent().setBackgroundTexture(m_params.backgroundTexture);

    m_params.renderer->getCamera().setPosition({ 0.f, 0.f, -5.f });

    m_params.renderer->swapCubeBuffers(m_params.cubes);

    m_params.renderer->render();

    m_params.context->renderFrameEnd();

    m_renderFinished.set_value();
}

EndNamespaceOlympus
