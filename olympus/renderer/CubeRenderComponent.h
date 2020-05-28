#pragma once

#include <vector>

#include <glad/glad.h>

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
    CubeRenderComponent();
    ~CubeRenderComponent();

    OlyPerfectForwardInContainer(renderCubes, m_cubes.frontBuffer())

    void render(const Camera& camera);

    void setDebugMode(bool debugMode) { m_debugMode = debugMode; }
    bool isDebugMode() const { return m_debugMode; }

    void swapBuffers() { m_cubes.swapBuffersSafe(); }
    void clearBackBuffer() { m_cubes.clearBackBuffer(); }

private:
    DoubleBuffer<std::vector<Cube>> m_cubes;

    bool m_debugMode{ false };

    GLuint m_vertexArrayID{ 0 };
    GLuint m_vertexBufferID{ 0 };

    ShaderProgram m_debugShader;
    ShaderProgram m_shader;
};

EndNamespaceOlympus
