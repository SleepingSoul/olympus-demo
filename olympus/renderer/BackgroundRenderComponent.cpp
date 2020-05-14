#include "BackgroundRenderComponent.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <render_defs.h>
#include <Texture.h>

BeginNamespaceOlympus

namespace
{
    // Fullscreen quad

    const GLfloat Vertices[] = {
        -1.f, -1.f, 0.f, 0.f, 0.f,
         1.f, -1.f, 0.f, 1.f, 0.f,
         1.f,  1.f, 0.f, 1.f, 1.f,
        -1.f, -1.f, 0.f, 0.f, 0.f,
         1.f,  1.f, 0.f, 1.f, 1.f,
        -1.f,  1.f, 0.f, 0.f, 1.f
    };

    const char* const BackgroundVS = "data/shaders/background.vs";
    const char* const BackgroundFS = "data/shaders/background.fs";

    const char* const TransformName = "transform";
    const char* const TextureName = "ourTexture";
}

BackgroundRenderComponent::BackgroundRenderComponent()
    : m_shader(BackgroundVS, BackgroundFS)
{
    glGenVertexArrays(1, &m_vertexArrayID);

    glBindVertexArray(m_vertexArrayID);

    glGenBuffers(1, &m_vertexBufferID);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    const GLuint verticeslayoutID = 0;
    const GLuint texCoordsLayoutID = 1;

    const auto stride = 5 * sizeof(GL_FLOAT);

    glVertexAttribPointer(verticeslayoutID, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(verticeslayoutID);
    glVertexAttribPointer(texCoordsLayoutID, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(texCoordsLayoutID);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

BackgroundRenderComponent::~BackgroundRenderComponent()
{
    glDeleteBuffers(1, &m_vertexBufferID);
    glDeleteVertexArrays(1, &m_vertexArrayID);
}

void BackgroundRenderComponent::render()
{
    if (!m_texture)
    {
        return;
    }

    const auto textureID = m_texture->getID();

    m_shader.use();

    glActiveTexture(GL_TEXTURE0);
    m_shader.setInt(TextureName, 0);

    glBindTexture(GL_TEXTURE_2D, textureID);

    std::array<GLint, 4> viewport;
    glGetIntegerv(GL_VIEWPORT, viewport.data());
    const auto viewportWidth = viewport[2];
    const auto viewportHeight = viewport[3];

    const GLint textureWidth = m_texture->getWidth();
    const GLint textureHeight = m_texture->getHeight();

    if (!m_scaleTexture && (viewportWidth != textureWidth || viewportHeight != textureHeight))
    {
        const auto transform = glm::scale(glm::identity<glm::mat4>(), { textureWidth / viewportWidth, textureHeight / viewportHeight, 1.f });

        m_shader.setMatrix4f(TransformName, glm::value_ptr(transform));
    }
    else
    {
        const auto identity = glm::identity<glm::mat4>();
        m_shader.setMatrix4f(TransformName, glm::value_ptr(identity));
    }

    glBindVertexArray(m_vertexArrayID);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);

    m_shader.unuse();
}

EndNamespaceOlympus
