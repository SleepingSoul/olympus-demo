#include <pch.h>
#include "EngineImpl.h"

#include <managers/CommandLineManager.h>

#include <DelegateJob.h>
#include <InitializeRenderJob.h>
#include <RenderDrawCallsJob.h>

BeginNamespaceOlympus

EngineImpl::EngineImpl()
{
    logging::debug("Initializing engine in main thread with id: {}", std::this_thread::get_id());

    m_profilerFile = olyCommandLineManager.getString(oly::CommandLineOptions::SaveProfile);

#ifndef BUILD_WITH_EASY_PROFILER
    if (m_profilerFile)
    {
        oly::logging::warning("Option '--saveprofile' will be ignored because it is not a profile build.");
    }

    m_profilerFile.reset();
#endif

    if (m_profilerFile)
    {
        EASY_PROFILER_ENABLE;
    }
    else
    {
        profiler::startListen();
    }

    InitializeRenderJob::InitParameters parameters{ m_openGLGLFWContext, m_openGLVoxelRenderer };

    auto initRenderJob = std::make_unique<InitializeRenderJob>(parameters);

    auto renderInitSuccessFuture = initRenderJob->getSuccessFuture();

    m_jobSystem.addJob(std::move(initRenderJob));

    EASY_BLOCK("Waiting for render initialization");

    m_successfulInitialization = renderInitSuccessFuture.get();

    EASY_END_BLOCK;

    logging::info("Render initialization result is: {}", m_successfulInitialization);
}

int EngineImpl::run()
{
    std::vector<oly::VoxelDrawCall> rdc(1);

    rdc[0].position = { -0.5f, 0.f, 0.f };
    rdc[0].rotationVec = { 0.2f, 0.2f, 0.2f };
    rdc[0].angle = 30.f;

    //m_openGLGLFWContext->addKeyboardCallback(GLFW_KEY_W, [&voxel = rdc.front()](int scancode, int action, int mode)
    //{
    //    if (action == GLFW_PRESS)
    //    {
    //        voxel.position.y += 0.2f;
    //    }
    //});

    //m_openGLGLFWContext->addKeyboardCallback(GLFW_KEY_S, [&voxel = rdc.front()](int scancode, int action, int mode)
    //{
    //    if (action == GLFW_PRESS)
    //    {
    //        voxel.position.y -= 0.2f;
    //    }
    //});

    //m_openGLGLFWContext->addKeyboardCallback(GLFW_KEY_A, [&voxel = rdc.front()](int scancode, int action, int mode)
    //{
    //    if (action == GLFW_PRESS)
    //    {
    //        voxel.position.x -= 0.2f;
    //    }
    //});

    //m_openGLGLFWContext->addKeyboardCallback(GLFW_KEY_D, [&voxel = rdc.front()](int scancode, int action, int mode)
    //{
    //    if (action == GLFW_PRESS)
    //    {
    //        voxel.position.x += 0.2f;
    //    }
    //});

    while (m_openGLGLFWContext->windowShoudNotClose())
    {
        EASY_BLOCK("frame");

        m_openGLGLFWContext->onFrameStart();

        std::vector<oly::VoxelDrawCall> rdc(1);

        rdc[0].position = { -0.5f, 0.f, 0.f };
        rdc[0].rotationVec = { 0.2f, 0.2f, 0.2f };
        rdc[0].angle = 30.f;

        RenderDrawCallsJob::InitParameters parameters{ m_openGLGLFWContext, m_openGLVoxelRenderer, std::move(rdc) };
        auto renderDrawCallsJob = std::make_unique<RenderDrawCallsJob>(std::move(parameters));

        auto renderFinishedFuture = renderDrawCallsJob->getRenderFinishedFuture();

        m_jobSystem.addJob(std::move(renderDrawCallsJob));

        EASY_BLOCK("Wait for render draw calls job to finish");

        renderFinishedFuture.wait();

        EASY_END_BLOCK;

        m_openGLGLFWContext->onFrameEnd();
    }

    auto deinitRenderJob = std::make_unique<DelegateJobWithPromise>(
        "Deinitialize render",
        JobAffinity::Render,
        [m_openGLGLFWContext = std::move(m_openGLGLFWContext), m_openGLVoxelRenderer = std::move(m_openGLVoxelRenderer)]() mutable
    {
        EASY_BLOCK("Render deinitialization");

        // Calling the destructors in the render thread
        // Probably we could somehow store this objects in the render thread?
        m_openGLVoxelRenderer.reset();
        m_openGLGLFWContext.reset();
    }
    );

    auto deinitRenderJobFinishedFuture = deinitRenderJob->getJobEndFuture();

    m_jobSystem.addJob(std::move(deinitRenderJob));

    EASY_BLOCK("Waiting for render to deinitialize");

    deinitRenderJobFinishedFuture.wait();

    EASY_END_BLOCK;

    if (m_profilerFile)
    {
        const auto numCapturedFrames = profiler::dumpBlocksToFile(m_profilerFile->c_str());

        if (numCapturedFrames == 0)
        {
            olyError("Profiler captured 0 frames.");
        }
    }

    return 0;
}

EndNamespaceOlympus
