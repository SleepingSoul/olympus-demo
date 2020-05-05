#include <pch.h>
#include "EngineImpl.h"

#include <managers/CommandLineManager.h>

#include <DelegateJob.h>
#include <InitializeRenderJob.h>

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

    EASY_BLOCK("Waiting for render initialization in the main thread");

    m_successfulInitialization = renderInitSuccessFuture.get();

    EASY_END_BLOCK;

    logging::debug("Render initialization result it: {}", m_successfulInitialization);
}

int EngineImpl::run()
{
    if (m_profilerFile)
    {
        const auto numCapturedFrames = profiler::dumpBlocksToFile(m_profilerFile->c_str());

        if (numCapturedFrames == 0)
        {
            olyError("Profiler captured 0 frames.");
        }
    }

    return 0;

    std::vector<oly::VoxelDrawCall> rdc(1);

    rdc[0].position = { -0.5f, 0.f, 0.f };
    rdc[0].rotationVec = { 0.2f, 0.2f, 0.2f };
    rdc[0].angle = 30.f;

    m_openGLGLFWContext->addKeyboardCallback(GLFW_KEY_W, [&voxel = rdc.front()](int scancode, int action, int mode)
    {
        if (action == GLFW_PRESS)
        {
            voxel.position.y += 0.2f;
        }
    });

    m_openGLGLFWContext->addKeyboardCallback(GLFW_KEY_S, [&voxel = rdc.front()](int scancode, int action, int mode)
    {
        if (action == GLFW_PRESS)
        {
            voxel.position.y -= 0.2f;
        }
    });

    m_openGLGLFWContext->addKeyboardCallback(GLFW_KEY_A, [&voxel = rdc.front()](int scancode, int action, int mode)
    {
        if (action == GLFW_PRESS)
        {
            voxel.position.x -= 0.2f;
        }
    });

    m_openGLGLFWContext->addKeyboardCallback(GLFW_KEY_D, [&voxel = rdc.front()](int scancode, int action, int mode)
    {
        if (action == GLFW_PRESS)
        {
            voxel.position.x += 0.2f;
        }
    });

    while (m_openGLGLFWContext->windowShoudNotClose())
    {
        EASY_BLOCK("frame");
        m_openGLGLFWContext->onFrameStart();

        m_openGLVoxelRenderer->setClearColor(glm::vec4{ 0.f, 0.f, 0.f, 1.f });

        m_openGLVoxelRenderer->setCameraPosition({ 0.f, 0.f, -5.f });
        m_openGLVoxelRenderer->setRenderField(m_openGLGLFWContext->getWindowSize());
        m_openGLVoxelRenderer->setDebugRender(true);

        m_openGLVoxelRenderer->renderVoxels(rdc);

        m_openGLGLFWContext->onFrameEnd();
    }

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
