#include <pch.h>
#include "EngineImpl.h"

#include <managers/CommandLineManager.h>

#include <DelegateJob.h>
#include <RenderFrameJob.h>

BeginNamespaceOlympus

EngineImpl::EngineImpl()
{
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

    // GLFW should be initialized from the main thread
    EASY_BLOCK("GLFW initialization");
    initGLFWContext();
    EASY_END_BLOCK;

    m_listener.start();

    m_openGLGLFWContext->setThreadContext(true);

    m_openGLVoxelRenderer = std::make_shared<OpenGLRenderer>();

    EASY_BLOCK("Textures preload");
    m_texStorage.preloadAllTextures();
    EASY_END_BLOCK;

    m_openGLGLFWContext->setThreadContext(false);

    m_successfulInitialization = true;
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

    cv::Mat m;

    while (m_openGLGLFWContext->windowShoudNotClose())
    {
        EASY_BLOCK("frame");

        m_openGLGLFWContext->onFrameStart();

        auto renderFinishedFuture = prepeareAndSendRenderFrameJob();

        EASY_BLOCK("Wait for render frame job to finish", profiler::colors::DarkBlue);
        renderFinishedFuture.get();
        EASY_END_BLOCK;

        m_openGLGLFWContext->onFrameEnd();
    }

    EASY_BLOCK("Stopping job system", profiler::colors::LightBlue);
    logging::debug("Stopping job system");
    m_jobSystem.stop();
    EASY_END_BLOCK;

    EASY_BLOCK("Stopping async videostream listener", profiler::colors::LightBlue);
    logging::debug("Stopping async videostream listener");
    m_listener.stop();
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

void EngineImpl::initGLFWContext()
{
    EASY_FUNCTION();

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
    catch (const oly::InfoException& e)
    {
        olyError("Failed to create OpenGL GLFW context: {}", e.what());
        return;
    }
}

[[nodiscard]] std::future<void> EngineImpl::prepeareAndSendRenderFrameJob()
{
    // Doing it here because "getWindowSize" is allowed only from the main thread
    m_openGLVoxelRenderer->setRenderField(m_openGLGLFWContext->getWindowSize());

    std::vector<oly::VoxelDrawCall> drawCalls(1);

    RenderFrameJob::InitParameters parameters{ m_openGLGLFWContext, m_openGLVoxelRenderer, std::move(drawCalls),
        m_texStorage.getTexture(TextureID::NoSignal) };
    auto renderFrameJob = std::make_unique<RenderFrameJob>(std::move(parameters));

    auto renderFinishedFuture = renderFrameJob->getRenderFinishedFuture();

    m_jobSystem.addJob(std::move(renderFrameJob));

    return renderFinishedFuture;
}

EndNamespaceOlympus
