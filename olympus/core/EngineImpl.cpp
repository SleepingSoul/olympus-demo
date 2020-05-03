#include <pch.h>
#include "EngineImpl.h"

#include <managers/CommandLineManager.h>

#include <DelegateJob.h>

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

    OpenGLGLFWContext::InitParameters params{};

    params.verMajor = 3;
    params.verMinor = 3;
    params.windowWidth = olyCommandLineManager.getLong(oly::CommandLineOptions::Width).value_or(800);
    params.windowHeight = olyCommandLineManager.getLong(oly::CommandLineOptions::Height).value_or(600);
    params.windowTitle = "olympus";
    params.glslVersion = "#version 330 core";

    try
    {
        m_openGLGLFWContext = std::make_unique<OpenGLGLFWContext>(params);
    }
    catch (const oly::InfoException & e)
    {
        oly::logging::critical("Failed to create OpenGL GLFW context: {}", e.what());
        return;
    }

    m_openGLVoxelRenderer = std::make_unique<OpenGLVoxelRenderer>();

    m_successfulInitialization = true;
}

int EngineImpl::run()
{
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

    m_jobSystem.addJob(std::make_unique<DelegateJob>("Sleep test job", JobAffinity::Generic, []
    {
        EASY_BLOCK("Sleep job");
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }));

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
