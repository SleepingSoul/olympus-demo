#pragma once

#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <utils/macros.h>

#include <ShaderProgram.h>
#include <BackgroundRenderComponent.h>
#include <CubeRenderComponent.h>
#include <Camera.h>

BeginNamespaceOlympus

class OpenGLRenderer
{
public:
    OpenGLRenderer();

    void swapCubeBuffers(std::vector<Cube>& cubes);

    Camera& getCamera() { return m_camera; }
    BackgroundRenderComponent& getBackgroundRenderComponent() { return m_backgroundRenderComponent; }
    CubeRenderComponent& getCubeRenderComponent() { return m_cubeRenderComponent; }

    void setRenderField(const glm::ivec2 renderField) { m_renderField = renderField; }

    void render();

private:
    BackgroundRenderComponent m_backgroundRenderComponent;
    CubeRenderComponent m_cubeRenderComponent;

    std::vector<Cube> m_cubes;

    Camera m_camera;

    glm::ivec2 m_renderField;
};

EndNamespaceOlympus
