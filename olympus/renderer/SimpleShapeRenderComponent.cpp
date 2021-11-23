#include "SimpleShapeRenderComponent.h"

#include <array>

#include <easy/profiler.h>

BeginNamespaceOlympus

namespace
{
    const size_t MaxPoints = 100;

    const char* const PointsVS = "data/shaders/points2d.vs";
    const char* const PointsFS = "data/shaders/points2d.fs";

    const char* const ColorNameFormat = "colors[%zu]";
    const char* const PositionNameFormat = "positions[%zu]";
    const char* const SizeNameFormat = "sizes[%zu]";
    const char* const ViewNameFormat = "view[%zu]";
    const char* const FBSizeName = "FramebufferSize";
}

SimpleShapeRenderComponent::SimpleShapeRenderComponent()
    : m_2DpointsShader(PointsVS, PointsFS)
{
    glGenVertexArrays(1, &m_vertexArrayID);
    //glBindVertexArray(m_vertexArrayID);

    //glGenBuffers(1, &m_vertexBufferID);
    //glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);

    //GLfloat point = 0.f;

    //glBufferData(GL_ARRAY_BUFFER, sizeof(point), &point, GL_STATIC_DRAW);
    //glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), reinterpret_cast<void*>(0));
    //glEnableVertexAttribArray(0);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SimpleShapeRenderComponent::~SimpleShapeRenderComponent()
{
    //glDeleteBuffers(1, &m_vertexBufferID);
    glDeleteVertexArrays(1, &m_vertexArrayID);
}

void SimpleShapeRenderComponent::render()
{
    m_2DpointsShader.use();

    std::array<char, 32> buffer = { '\0' };

    std::array<GLint, 4> viewport;
    glGetIntegerv(GL_VIEWPORT, viewport.data());
    const auto viewportWidth = viewport[2];
    const auto viewportHeight = viewport[3];

    m_2DpointsShader.setVec2i(FBSizeName, glm::ivec2(viewportWidth, viewportHeight));

    const auto size = std::min(m_2Dpoints.backBuffer().size(), MaxPoints);

    EASY_BLOCK("Transfering data to shader", profiler::colors::Red300);
    for (size_t i = 0; i < size; ++i)
    {
        const auto& point = m_2Dpoints.backBuffer()[i];

        std::sprintf(buffer.data(), ColorNameFormat, i);
        m_2DpointsShader.setVec4f(buffer.data(), point.color);
        
        std::sprintf(buffer.data(), PositionNameFormat, i);
        m_2DpointsShader.setVec3f(buffer.data(), point.position);

        std::sprintf(buffer.data(), SizeNameFormat, i);
        m_2DpointsShader.setFloat(buffer.data(), point.size);

        if (&point.viewmodel.at<float>(0)) [[likely]]
        {
            std::sprintf(buffer.data(), ViewNameFormat, i);
            m_2DpointsShader.setMatrix4f(buffer.data(), &point.viewmodel.at<float>(0));
        }
        else [[unlikely]]
        {
            logging::error("[SimpleShapeRenderComponent] View Model matrix is invalid, skipping shader update.");
        }
    }
    EASY_END_BLOCK;

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glBindVertexArray(m_vertexArrayID);
    glDrawArraysInstanced(GL_POINTS, 0, 1, size);
    glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);

    m_2DpointsShader.unuse();
}

size_t SimpleShapeRenderComponent::getMaxPoints() const
{
    return MaxPoints;
}

EndNamespaceOlympus
