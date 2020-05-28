#pragma once

#include <glad/glad.h>

#include <utils/macros.h>

#include <ShaderProgram.h>

BeginNamespaceOlympus

class Texture;

class BackgroundRenderComponent
{
public:
    BackgroundRenderComponent();
    ~BackgroundRenderComponent();

    void setBackgroundTexture(Texture* texture) { m_texture = texture; }
    void setScaleTexture(bool scaleTexture) { m_scaleTexture = scaleTexture; }

    void render();

    void swapBuffers() {}

private:
    GLuint m_vertexArrayID{ 0 };
    GLuint m_vertexBufferID{ 0 };

    Texture* m_texture{ nullptr };
    bool m_scaleTexture{ false };

    ShaderProgram m_shader;
};

EndNamespaceOlympus
