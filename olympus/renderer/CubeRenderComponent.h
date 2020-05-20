#pragma once

#include <vector>

#include <glad/glad.h>

#include <utils/macros.h>

#include <ShaderProgram.h>
#include <Cube.h>

BeginNamespaceOlympus

class Camera;

class CubeRenderComponent
{
public:
    CubeRenderComponent();
    ~CubeRenderComponent();

    void renderCubes(const Camera& camera, std::vector<Cube>& cubes);

    void setDebugMode(bool debugMode) { m_debugMode = debugMode; }
    bool isDebugMode() const { return m_debugMode; }

private:
    bool m_debugMode{ false };

    GLuint m_vertexArrayID{ 0 };
    GLuint m_vertexBufferID{ 0 };

    ShaderProgram m_debugShader;
    ShaderProgram m_shader;
};

EndNamespaceOlympus
