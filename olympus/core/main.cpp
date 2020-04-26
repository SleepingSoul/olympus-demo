#include <iostream>
#include <easy/profiler.h>
#include <utils/olyerror.h>
#include <logging/logging.h>
#include <managers/CommandLineManager.h>
#include <OpenGLVoxelRenderer.h>
#include <OpenGLGLFWContext.h>

int main(int argc, char** argv)
{
    oly::logging::initialize();
    olyCommandLineManager.initialize(argc, argv);

    const auto width = olyCommandLineManager.getLong("--width");
    const auto height = olyCommandLineManager.getLong("--height");

    auto profilerFile = olyCommandLineManager.getString("--saveprofile");

#ifndef BUILD_WITH_EASY_PROFILER
    if (profilerFile)
    {
        oly::logging::warning("Option '--saveprofile' will be ignored because it is not a profile build.");
    }

    profilerFile.reset();
#endif

    if (profilerFile)
    {
        EASY_PROFILER_ENABLE;
    }
    else
    {
        profiler::startListen();
    }

    if (!width || !height)
    {
        olyError("Wrong command line values provided for initial window size.");
        return 0;
    }

    // Creating and initializing OpenGL context (should be always initialized before anything else opengl-ish).
    oly::OpenGLGLFWContext::InitParameters params;
    params.verMajor = 3;
    params.verMinor = 3;
    params.windowWidth = width.value_or(800);
    params.windowHeight = height.value_or(600);
    params.windowTitle = "olympus";
    params.glslVersion = "#version 330 core";

    std::unique_ptr<oly::OpenGLGLFWContext> context;

    try
    {
        context = std::make_unique<oly::OpenGLGLFWContext>(params);
    }
    catch (const oly::InfoException & e)
    {
        oly::logging::critical("Failed to create OpenGL GLFW context: {}", e.what());
        return -1;
    }

    // After context is initialized, we can create an OpenGL Renderer to render using the existing context. Only 1 contexts exists
    // and renderer will render into this context.
    oly::OpenGLVoxelRenderer renderer;

    std::vector<oly::VoxelDrawCall> rdc(1);

    rdc[0].position = { -0.5f, 0.f, 0.f };
    rdc[0].rotationVec = { 0.2f, 0.2f, 0.2f };
    rdc[0].angle = 30.f;

    context->addKeyboardCallback(GLFW_KEY_W, [&voxel = rdc.front()](int scancode, int action, int mode)
    {
        if (action == GLFW_PRESS)
        {
            voxel.position.y += 0.2f;
        }
    });

    context->addKeyboardCallback(GLFW_KEY_S, [&voxel = rdc.front()](int scancode, int action, int mode)
    {
        if (action == GLFW_PRESS)
        {
            voxel.position.y -= 0.2f;
        }
    });

    context->addKeyboardCallback(GLFW_KEY_A, [&voxel = rdc.front()](int scancode, int action, int mode)
    {
        if (action == GLFW_PRESS)
        {
            voxel.position.x -= 0.2f;
        }
    });

    context->addKeyboardCallback(GLFW_KEY_D, [&voxel = rdc.front()](int scancode, int action, int mode)
    {
        if (action == GLFW_PRESS)
        {
            voxel.position.x += 0.2f;
        }
    });

    while (context->windowShoudNotClose())
    {
        EASY_BLOCK("frame");
        context->onFrameStart();

        renderer.setClearColor(glm::vec4{ 0.f, 0.f, 0.f, 1.f });

        renderer.setCameraPosition({ 0.f, 0.f, -5.f });
        renderer.setRenderField(context->getWindowSize());
        renderer.setDebugRender(true);

        renderer.renderVoxels(rdc);

        context->onFrameEnd();
    }

    if (profilerFile)
    {
        const auto numCapturedFrames = profiler::dumpBlocksToFile(profilerFile->c_str());

        if (numCapturedFrames == 0)
        {
            olyError("Profiler captured 0 frames.");
        }
    }

    return 0;
}