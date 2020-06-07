#pragma once

#include <vector>
#include <memory_resource>

#include <glad/glad.h>

#include <opencv2/opencv.hpp>

#include <utils/macros.h>
#include <utils/forward.h>
#include <containers/DoubleBuffer.h>

#include <ShaderProgram.h>
#include <Cube.h>

BeginNamespaceOlympus

class Camera;

class CubeRenderComponent
{
public:
    struct CubeWorld
    {
        std::vector<Cube> cubes;
        cv::Mat modelViewMatrix;
        cv::Mat projectionMatrix;
    };

    CubeRenderComponent();
    ~CubeRenderComponent();

    OlyPerfectForwardInContainer(renderCubes, m_cubeWorlds.frontBuffer())

    void render(const Camera& camera);

    void setDebugMode(bool debugMode) { m_debugMode = debugMode; }
    bool isDebugMode() const { return m_debugMode; }

    void swapBuffers() { m_cubeWorlds.swapBuffersSafe(); }
    void clearBackBuffer() { m_cubeWorlds.clearBackBuffer(); }

private:
    DoubleBuffer<std::vector<CubeWorld>> m_cubeWorlds;

    bool m_debugMode{ false };

    GLuint m_vertexArrayID{ 0 };
    GLuint m_vertexBufferID{ 0 };

    ShaderProgram m_debugShader;
    ShaderProgram m_shader;
};

EndNamespaceOlympus
