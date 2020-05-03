#include "OpenGLVoxelRenderer.h"
#include <string_view>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <easy/profiler.h>

BeginNamespaceOlympus

namespace Shader
{
    const std::string_view Model = "model";
    const std::string_view View = "view";
    const std::string_view Projection = "projection";
    const std::string_view Color = "color";
}

namespace Voxel
{
    namespace
    {
        const float Vertices[] = {
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f
        };

        const size_t NumVertices = sizeof(Vertices) / sizeof(decltype(*Vertices)) / 3;
    }
}

namespace Debug
{
    namespace
    {
        const glm::vec4 Color{ 0.f, 1.f, 0.f, 1.f };
        const std::string_view VSPath = "data/debug_shader.vs";
        const std::string_view FSPath = "data/debug_shader.fs";
    }
}

OpenGLVoxelRenderer::OpenGLVoxelRenderer()
    : m_debugShader(Debug::VSPath, Debug::FSPath)
    , m_debugRender(false)
    , m_cameraPos(0.f, 0.f, 0.f)
    , m_cameraAngleDeg(45.f)
    , m_nearDistance(1.f)
    , m_farDistance(100.f)
    , m_renderField{1.f, 1.f}
{
    glClearColor(0.f, 0.f, 0.f, 1.f);

    glGenVertexArrays(1, &m_voxelVAO);
    glGenBuffers(1, &m_voxelVBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_voxelVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Voxel::Vertices), Voxel::Vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_voxelVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
}

void OpenGLVoxelRenderer::setClearColor(const glm::vec4& rgbaColor)
{
    EASY_FUNCTION();

    glClearColor(rgbaColor.r, rgbaColor.g, rgbaColor.b, rgbaColor.a);
}

void OpenGLVoxelRenderer::renderVoxels(std::vector<VoxelDrawCall>& voxels)
{
    EASY_FUNCTION(profiler::colors::Red);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ShaderProgram& shader = m_debugRender ? m_debugShader : m_voxelShader;

    shader.use();

    {
        const auto view = glm::translate(glm::identity<glm::mat4>(), m_cameraPos);
        shader.setMatrix4f(Shader::View, glm::value_ptr(view));
    }

    {
        const auto projection = glm::perspectiveFov(glm::radians(m_cameraAngleDeg), m_renderField.x, m_renderField.y
            , m_nearDistance, m_farDistance);
        shader.setMatrix4f(Shader::Projection, glm::value_ptr(projection));
    }
    
    EASY_BLOCK("Render draw calls", profiler::colors::Red100);
    for (const auto& voxel : voxels)
    {
        EASY_BLOCK("Render single voxel", profiler::colors::Yellow);

        auto model = glm::identity<glm::mat4>();
        model = glm::translate(model, voxel.position);
        model = glm::rotate(model, voxel.angle, voxel.rotationVec);

        shader.setMatrix4f(Shader::Model, glm::value_ptr(model));

        if (m_debugRender)
        {
            shader.setVec4f(Shader::Color, Debug::Color);

            debugRenderVoxel();
        }
        else
        {
            // TODO: impl
        }
    }
}

void OpenGLVoxelRenderer::debugRenderVoxel()
{
    for (GLuint i = 0; i < Voxel::NumVertices; i += 3)
    {
        glDrawArrays(GL_LINE_LOOP, i, 3);
    }

    glPointSize(5.f);
    glDrawArrays(GL_POINTS, 0, Voxel::NumVertices);
}

EndNamespaceOlympus
