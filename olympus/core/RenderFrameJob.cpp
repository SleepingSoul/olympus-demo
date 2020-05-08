#include <pch.h>
#include "RenderFrameJob.h"
#include <JobAffinity.h>
#include <OpenGLGLFWContext.h>
#include <OpenGLVoxelRenderer.h>

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

    m_params.renderer->setClearColor(glm::vec4{ 0.f, 0.f, 0.f, 1.f });

    m_params.renderer->setCameraPosition({ 0.f, 0.f, -5.f });

    m_params.renderer->setDebugRender(true);

    m_params.renderer->renderVoxels(m_params.drawCalls);

    m_params.context->renderFrameEnd();

    m_renderFinished.set_value();
}

EndNamespaceOlympus
