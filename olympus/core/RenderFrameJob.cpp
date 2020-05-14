#include <pch.h>
#include "RenderFrameJob.h"
#include <Engine.h>
#include <AsyncVideostreamListener.h>
#include <JobAffinity.h>
#include <OpenGLGLFWContext.h>
#include <OpenGLVoxelRenderer.h>
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
    EASY_FUNCTION("Num draw calls = %zu", m_params.drawCalls.size(), profiler::colors::Red);

    m_params.context->renderFrameStart();

    auto& listener = olyEngine.getAsyncVideostreamListener();

    if (listener.isListening())
    {
        const auto lastFrame = listener.getLatestFrame();

        if (!lastFrame.empty())
        {
            const auto size = lastFrame.size();
            m_params.backgroundTexture->hotReset(lastFrame.ptr(), size.width, size.height, GL_BGR);
        }
    }

    m_params.renderer->getBackgroundRenderComponent().setBackgroundTexture(m_params.backgroundTexture);

    m_params.renderer->setClearColor(glm::vec4{ 0.f, 0.f, 0.f, 1.f });

    m_params.renderer->setCameraPosition({ 0.f, 0.f, -5.f });

    m_params.renderer->renderVoxels(m_params.drawCalls);

    m_params.context->renderFrameEnd();

    m_renderFinished.set_value();
}

EndNamespaceOlympus
