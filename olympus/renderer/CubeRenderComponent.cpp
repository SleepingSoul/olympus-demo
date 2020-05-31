#include "CubeRenderComponent.h"

#include <easy/profiler.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <extra_std/extra_std.h>

#include <utils/olyerror.h>

#include <render_defs.h>
#include <Camera.h>
#include <Texture.h>

BeginNamespaceOlympus

namespace
{
    const GLfloat CubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    const size_t NumVertices = sizeof(CubeVertices) / sizeof(decltype(*CubeVertices)) / 5;

    const char* const DebugCubeVS = "data/shaders/cube_shader_d.vs";
    const char* const DebugCubeFS = "data/shaders/cube_shader_d.fs";

    const char* const CubeVS = "data/shaders/cube_shader.vs";
    const char* const CubeFS = "data/shaders/cube_shader.fs";

    const char* const Model = "model";
    const char* const View = "view";
    const char* const Projection = "projection";
    const char* const Color = "color";
    const char* const TextureName = "textureID";

    const glm::vec4 DebugColor{ 0.f, 1.f, 0.f, 1.f };
}

CubeRenderComponent::CubeRenderComponent()
    : m_debugShader(DebugCubeVS, DebugCubeFS)
    , m_shader(CubeVS, CubeFS)
{
    glGenVertexArrays(1, &m_vertexArrayID);

    glBindVertexArray(m_vertexArrayID);

    glGenBuffers(1, &m_vertexBufferID);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);

    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

    const GLuint verticesLayoutID = 0;
    const GLuint texCoordsLayoutID = 1;

    const auto stride = 5 * sizeof(GL_FLOAT);

    glVertexAttribPointer(verticesLayoutID, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(verticesLayoutID);
    glVertexAttribPointer(texCoordsLayoutID, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(texCoordsLayoutID);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

CubeRenderComponent::~CubeRenderComponent()
{
    glDeleteBuffers(1, &m_vertexBufferID);
    glDeleteVertexArrays(1, &m_vertexArrayID);
}

void CubeRenderComponent::render(const Camera& camera)
{
    EASY_FUNCTION(profiler::colors::Red);

    const auto modelView = camera.getARModelViewMatrix();
    const auto projection = camera.getARProjectionMatrix();

    if (modelView.size() != cv::Size(4, 4) || modelView.type() != CV_32F || projection.size() != cv::Size(4, 4) || projection.type() != CV_32F)
    {
        logging::warning("[CubeRendererComponent] Cannot perform render operation because input parameters are invalid.");
        return;
    }

    auto& shader = m_debugMode ? m_debugShader : m_shader;

    shader.use();

    shader.setMatrix4f(View, &modelView.at<float>(0));

    std::array<GLint, 4> viewport;
    glGetIntegerv(GL_VIEWPORT, viewport.data());
    const auto viewportWidth = viewport[2];
    const auto viewportHeight = viewport[3];

    //auto projection = glm::perspectiveFov(
    //    glm::radians(51.f),
    //    static_cast<float>(viewportWidth),
    //    static_cast<float>(viewportHeight),
    //    0.1f,
    //    10.f);

    //auto projection = glm::identity<glm::mat4>();

    //const auto projection = glm::ortho(-1.f, 1.f, -1.f, 1.f, 0.f, 100.f);

    shader.setMatrix4f(Projection, &projection.at<float>(0));

    glBindVertexArray(m_vertexArrayID);

    glEnable(GL_DEPTH_TEST);

    for (const Cube& cube : m_cubes.backBuffer())
    {
        auto model = glm::identity<glm::mat4>();
        model = glm::translate(model, cube.position);
        model = glm::rotate(model, cube.rotationValueRad, cube.rotationAxis);
        model = glm::scale(model, glm::vec3(cube.edgeSize));

        shader.setMatrix4f(Model, glm::value_ptr(model));

        if (m_debugMode)
        {
            shader.setVec4f(Color, DebugColor);

            for (GLuint i = 0; i < NumVertices; i += 3)
            {
                glDrawArrays(GL_LINE_LOOP, i, 3);
            }

            glPointSize(5.f);
            glDrawArrays(GL_POINTS, 0, NumVertices);
        }
        else
        {
            if (!cube.face)
            {
                logging::warning("[CubeRenderComponent] No face specified for cube. Render skipped.");
                continue;
            }

            glActiveTexture(GL_TEXTURE0);
            shader.setInt(TextureName, 0);
            
            glBindTexture(GL_TEXTURE_2D, cube.face->getID());

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    glDisable(GL_DEPTH_TEST);

    shader.unuse();

    glBindVertexArray(0);
}

EndNamespaceOlympus
