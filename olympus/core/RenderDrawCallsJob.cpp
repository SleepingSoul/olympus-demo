#include <pch.h>
#include "RenderDrawCallsJob.h"
#include <JobAffinity.h>
#include <OpenGLGLFWContext.h>
#include <OpenGLVoxelRenderer.h>

BeginNamespaceOlympus

RenderDrawCallsJob::RenderDrawCallsJob(InitParameters&& initParams)
    : Base("Render draw calls", JobAffinity::Render)
    , m_params(std::move(initParams))
{}

std::future<void> RenderDrawCallsJob::getRenderFinishedFuture()
{
    return m_renderFinished.get_future();
}

void RenderDrawCallsJob::execute()
{
    EASY_FUNCTION("Num draw calls = %zu", m_params.drawCalls.size());

    JobExecutionGuard jeg(*this);

    m_params.context->renderFrameStart();

    m_params.renderer->setClearColor(glm::vec4{ 0.f, 0.f, 0.f, 1.f });

    m_params.renderer->setCameraPosition({ 0.f, 0.f, -5.f });
    m_params.renderer->setRenderField(m_params.context->getWindowSize());
    m_params.renderer->setDebugRender(true);

    m_params.renderer->renderVoxels(m_params.drawCalls);

    m_params.context->renderFrameEnd();
}

EndNamespaceOlympus