#include <pch.h>
#include "InitializeRenderJob.h"
#include <managers/CommandLineManager.h>
#include <JobAffinity.h>
#include <OpenGLGLFWContext.h>
#include <OpenGLVoxelRenderer.h>

BeginNamespaceOlympus

InitializeRenderJob::InitializeRenderJob(InitParameters initParams)
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

    OpenGLGLFWContext::InitParameters params{};

    params.verMajor = 3;
    params.verMinor = 3;
    params.windowWidth = olyCommandLineManager.getLong(oly::CommandLineOptions::Width).value_or(800);
    params.windowHeight = olyCommandLineManager.getLong(oly::CommandLineOptions::Height).value_or(600);
    params.windowTitle = "olympus";
    params.glslVersion = "#version 330 core";

    try
    {
        m_initialization.outContext = std::make_unique<OpenGLGLFWContext>(params);
    }
    catch (const oly::InfoException& e)
    {
        oly::logging::critical("Failed to create OpenGL GLFW context: {}", e.what());
        m_initSuccess.set_value(false);
        return;
    }

    m_initialization.outRenderer = std::make_unique<OpenGLVoxelRenderer>();
    m_initSuccess.set_value(true);
}

EndNamespaceOlympus
