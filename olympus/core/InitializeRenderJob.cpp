#include <pch.h>
#include "InitializeRenderJob.h"
#include <managers/CommandLineManager.h>
#include <JobAffinity.h>
#include <OpenGLGLFWContext.h>
#include <OpenGLVoxelRenderer.h>

BeginNamespaceOlympus

InitializeRenderJob::InitializeRenderJob(InitParameters&& initParams)
    : Base("Initialize render", JobAffinity::Render)
    , m_initialization(std::move(initParams))
{}

std::future<bool> InitializeRenderJob::getSuccessFuture()
{
    return m_initSuccess.get_future();
}

void InitializeRenderJob::execute()
{
    EASY_FUNCTION();

    JobExecutionGuard jeg(*this);

    m_initialization.outRenderer = std::make_unique<OpenGLVoxelRenderer>();
    m_initSuccess.set_value(true);
}

EndNamespaceOlympus
