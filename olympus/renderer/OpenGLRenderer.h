#pragma once

#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <utils/macros.h>

#include <ShaderProgram.h>
#include <BackgroundRenderComponent.h>
#include <CubeRenderComponent.h>
#include <SimpleShapeRenderComponent.h>
#include <Camera.h>

BeginNamespaceOlympus

class OpenGLRenderer
{
public:
    OpenGLRenderer();

    Camera& getCamera() { return m_camera; }
    auto& getBackgroundRenderComponent() { return m_backgroundRenderComponent; }
    auto& getCubeRenderComponent() { return m_cubeRenderComponent; }
    auto& getSimpleShapeRenderComponent() { return m_simpleShapeRenderComponent; }

    void setRenderField(const glm::ivec2 renderField) { m_renderField = renderField; }

    void render();

    void swapBuffers();

    void setDebugMode(bool debugMode);

private:
    BackgroundRenderComponent m_backgroundRenderComponent;
    CubeRenderComponent m_cubeRenderComponent;
    SimpleShapeRenderComponent m_simpleShapeRenderComponent;

    Camera m_camera;

    glm::ivec2 m_renderField;
};

EndNamespaceOlympus
