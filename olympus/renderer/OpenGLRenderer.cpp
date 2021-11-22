#include "OpenGLRenderer.h"
#include <string_view>

#include <Windows.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <easy/profiler.h>

#include <utils/threading_utils.h>
#include <utils/olyerror.h>

BeginNamespaceOlympus

extern "C"
{
    // Request high performance graphics (discrete GPU) on Laptops with AMD and Nvidia GPUs

    //__declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x0000'0001;
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x0000'0001;
}

OpenGLRenderer::OpenGLRenderer()
    : m_camera()
{}

void OpenGLRenderer::render()
{
    if (m_renderField == glm::ivec2{ 0, 0 })
    {
        return;
    }

    glViewport(0, 0, m_renderField.x, m_renderField.y);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    EASY_BLOCK("Render background", profiler::colors::Red100);
    m_backgroundRenderComponent.render();
    EASY_END_BLOCK;

    EASY_BLOCK("Render cubes", profiler::colors::Red200);
    //m_cubeRenderComponent.render(m_camera);
    EASY_END_BLOCK;

    EASY_BLOCK("Render models");
    m_anyModelRenderComponent.render(m_camera);
    EASY_END_BLOCK;

    EASY_BLOCK("Render 2D points");
    m_simpleShapeRenderComponent.render();
    EASY_END_BLOCK;
}

void OpenGLRenderer::swapBuffers()
{
    if (!threading::isMainThread())
    {
        olyError("[OpenGLRenderer] Potentialy unsafe operation: swapping buffers from the secondary thread.");
    }

    m_cubeRenderComponent.swapBuffers();
    m_simpleShapeRenderComponent.swapBuffers();
}

void OpenGLRenderer::clearBackBuffers()
{
    if (!threading::isMainThread())
    {
        olyError("[OpenGLRenderer] Potentialy unsafe operation: clearing render buffers from the secondary thread.");
    }

    m_cubeRenderComponent.clearBackBuffer();
    m_simpleShapeRenderComponent.clearBackBuffers();
}

void OpenGLRenderer::setDebugMode(bool debugMode)
{
    m_cubeRenderComponent.setDebugMode(debugMode);
}

EndNamespaceOlympus
