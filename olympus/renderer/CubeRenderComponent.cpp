#include "CubeRenderComponent.h"

#include <unordered_set>
#include <numeric>

#include <easy/profiler.h>
#include <easy/arbitrary_value.h>

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
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
    };

    const size_t NumVertices = sizeof(CubeVertices) / sizeof(decltype(*CubeVertices)) / 5;

    constexpr size_t MaxCubesBatch = 500;
    constexpr size_t UniformBufferCapacity = MaxCubesBatch * 16 * 4 + MaxCubesBatch * 4 * 4;
    constexpr size_t ModelsOffset = 0;
    constexpr size_t DiffusesOffset = ModelsOffset + MaxCubesBatch * 16 * 4;

    const char* const DebugCubeVS = "data/shaders/cube_shader_d.vs";
    const char* const DebugCubeFS = "data/shaders/cube_shader_d.fs";

    const char* const CubeVS = "data/shaders/cube_shader.vs";
    const char* const CubeFS = "data/shaders/cube_shader.fs";

    const char* const Model = "model";
    const char* const View = "view";
    const char* const Projection = "projection";
    const char* const Color = "color";
    const char* const TextureName = "faceTexture";
    const char* const ModelsNameFormat = "models[%zu]";
    const char* const DiffuseNameFormat = "diffuses[%zu]";
    const char* const BatchName = "batch";

    const glm::vec4 DebugColor{ 0.f, 1.f, 0.f, 1.f };
}

CubeRenderComponent::CubeRenderComponent()
    : m_debugShader(DebugCubeVS, DebugCubeFS)
    , m_shader(ShaderProgram::InitParameters{ CubeVS, "BatchBlock", UniformBufferCapacity, CubeFS })
{
    glGenVertexArrays(1, &m_vertexArrayID);

    glBindVertexArray(m_vertexArrayID);

    glGenBuffers(1, &m_vertexBufferID);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);

    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

    const GLuint verticesLayoutID = 0;
    const GLuint texCoordsLayoutID = 1;
    const GLuint normalsLayoutID = 2;

    const auto stride = 8 * sizeof(GL_FLOAT);

    glVertexAttribPointer(verticesLayoutID, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(verticesLayoutID);
    glVertexAttribPointer(texCoordsLayoutID, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(texCoordsLayoutID);
    glVertexAttribPointer(normalsLayoutID, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(normalsLayoutID);

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
    // TODO: refactor this function, looks very spaghetti-like... but still works

    EASY_FUNCTION(profiler::colors::Red);

    glBindVertexArray(m_vertexArrayID);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_SHADER_STORAGE_BLOCK);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto& shader = m_debugMode ? m_debugShader : m_shader;

    shader.use();

    std::array<GLint, 4> viewport;
    glGetIntegerv(GL_VIEWPORT, viewport.data());
    const auto viewportWidth = viewport[2];
    const auto viewportHeight = viewport[3];

    EASY_VALUE("All frame cubes", std::accumulate(m_cubeWorlds.backBuffer().cbegin(), m_cubeWorlds.backBuffer().cend(), size_t(0),
        [](const size_t sum, const CubeWorld& world)
        {
            return sum + world.cubes.size();
        }));

    for (auto& cubeWorld : m_cubeWorlds.backBuffer())
    {
        EASY_BLOCK("Cube world render", profiler::colors::Red600);

        EASY_VALUE("NumCubes", cubeWorld.cubes.size());

        const auto& modelView = cubeWorld.modelViewMatrix;
        const auto& projection = cubeWorld.projectionMatrix;
        auto& cubes = cubeWorld.cubes;

        if (modelView.size() != cv::Size(4, 4) || modelView.type() != CV_32F || projection.size() != cv::Size(4, 4) || projection.type() != CV_32F)
        {
            logging::warning("[CubeRendererComponent] Cannot perform render operation because input parameters are invalid.");
            return;
        }

        shader.setMatrix4f(View, &modelView.at<float>(0));

        shader.setMatrix4f(Projection, &projection.at<float>(0));

        if (!m_debugMode)
        {
            const auto removeIt = std::remove_if(cubes.begin(), cubes.end(), [](const Cube& cube) { return !cube.face; });
            cubes.erase(removeIt, cubes.end());

            std::sort(cubes.begin(), cubes.end(), [](const Cube& left, const Cube& right)
            {
                return left.face->getID() < right.face->getID();
            });

            auto batchIt = cubes.cbegin();
            auto batchEnd = std::find_if(batchIt, cubes.cend(), [currentBatchTexID = batchIt->face->getID()](const Cube& cube)
            {
                return cube.face->getID() != currentBatchTexID;
            });

            if (std::distance(batchIt, batchEnd) > MaxCubesBatch)
            {
                batchEnd = std::next(batchIt, MaxCubesBatch);
            }

            std::vector<float> models(MaxCubesBatch * 16, 0.f);
            std::vector<float> diffuses(MaxCubesBatch * 4, 0.f);

            while (true)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, batchIt->face->getID());
                shader.setInt(TextureName, 0);

                size_t index = 0;


                EASY_BLOCK("Transfer batch data to shader", profiler::colors::RedA400);
                for (; batchIt != batchEnd && std::distance(batchIt, batchEnd) <= MaxCubesBatch; ++batchIt)
                {
                    const auto& cube = *batchIt;

                    std::copy(glm::value_ptr(cube.diffuse), glm::value_ptr(cube.diffuse) + 3, diffuses.begin() + index * 4);

                    std::array<char, 32> buffer = { '\0' };

                    auto model = glm::identity<glm::mat4>();
                    model = glm::translate(model, cube.position);
                    model = glm::rotate(model, cube.rotationValueRad, cube.rotationAxis);
                    model = glm::scale(model, glm::vec3(cube.edgeSize));

                    std::copy(glm::value_ptr(model), glm::value_ptr(model) + 16, models.begin() + index * 16);

                    ++index;
                }

                m_shader.setInt(TextureName, 0);
                m_shader.setVertexUniformBufferSub(ModelsOffset, models.data(), models.size() * 4);
                m_shader.setVertexUniformBufferSub(DiffusesOffset, diffuses.data(), diffuses.size() * 4);

                EASY_END_BLOCK;

                EASY_BLOCK("Render batch", profiler::colors::RedA700);
                EASY_VALUE("Batch size", index);
                glDrawArraysInstanced(GL_TRIANGLES, 0, 36, index);
                EASY_END_BLOCK;

                batchIt = batchEnd;

                if (batchIt == cubes.cend())
                {
                    break;
                }

                batchEnd = std::find_if(batchIt, cubes.cend(), [batchTexID = batchIt->face->getID()](const Cube& cube)
                {
                    return cube.face->getID() != batchTexID;
                });

                if (std::distance(batchIt, batchEnd) > MaxCubesBatch)
                {
                    batchEnd = std::next(batchIt, MaxCubesBatch);
                }
            }
        }
        else
        {
            for (const Cube& cube : cubes)
            {
                auto model = glm::identity<glm::mat4>();
                model = glm::translate(model, cube.position);
                model = glm::rotate(model, cube.rotationValueRad, cube.rotationAxis);
                model = glm::scale(model, glm::vec3(cube.edgeSize));

                shader.setMatrix4f(Model, glm::value_ptr(model));

                shader.setVec4f(Color, DebugColor);

                for (GLuint i = 0; i < NumVertices; i += 3)
                {
                    glDrawArrays(GL_LINE_LOOP, i, 3);
                }

                glPointSize(5.f);
                glDrawArrays(GL_POINTS, 0, NumVertices);

            }
        }
    }

    glDisable(GL_SHADER_STORAGE_BLOCK);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    shader.unuse();

    glBindVertexArray(0);
}

EndNamespaceOlympus
