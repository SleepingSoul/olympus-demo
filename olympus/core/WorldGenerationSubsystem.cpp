#include <pch.h>
#include "WorldGenerationSubsystem.h"

#include <easy/profiler.h>

#include <PerlinNoise/PerlinNoise.hpp>

#include <utils/math.h>

#include <EngineImpl.h>

BeginNamespaceOlympus

WorldGenerationSubsystem::WorldGenerationSubsystem(EngineImpl& engine)
    : Base(engine)
{
    m_worldBuffer.reserve(5000);
}

void WorldGenerationSubsystem::update()
{
    EASY_FUNCTION(profiler::colors::Grey600);

    Base::update();

    auto& renderer = m_engine.getRenderer();

    const auto& markers = m_engine.getMarkers();

    for (const auto& marker : markers)
    {
        EASY_BLOCK("Generate world for one marker", profiler::colors::Grey700);

        CubeRenderComponent::CubeWorld cubeWorld;

        cubeWorld.modelViewMatrix = marker.modelviewMatrix.clone();
        cubeWorld.projectionMatrix = marker.projectionMatrix.clone();

        generateWorldToBuffer(static_cast<std::uint32_t>(marker.markerID), glm::vec3{});

        cubeWorld.cubes = m_worldBuffer;

        renderer.getCubeRenderComponent().renderCubes(std::move(cubeWorld));

        renderer.getAnyModelRenderComponent().setProjection(marker.projectionMatrix.clone());
        renderer.getAnyModelRenderComponent().setModelview(marker.modelviewMatrix.clone());
    }
}

void WorldGenerationSubsystem::makeWaterIfNeeded(Cube& cube, size_t index)
{
    const auto time = m_engine.getTimeFromStart();

    if (equalsWithAlpha(cube.position.z, 0.f, 0.001f))
    {
        cube.face = m_engine.getTextureStorage().getTexture(TextureID::WaterTransparent);
        cube.position.z += cube.edgeSize * static_cast<float>(std::sin((time + index) * 2)) / 2.f;
        cube.diffuse = glm::vec3(0.1f);
    }
    else
    {
        cube.face = m_engine.getTextureStorage().getTexture(TextureID::Crate);
    }
}

void WorldGenerationSubsystem::generateWorldToBuffer(std::uint32_t seed, const glm::vec3& eyePosition [[maybe_unused]])
{
    m_worldBuffer.clear();

    constexpr size_t mapSize = 30;
    constexpr float cubeEdge = 0.01f;

    const siv::BasicPerlinNoise<float> noiseEngine(seed);

    //std::array<std::array<float, mapSize>, mapSize> heightMap;

    for (int i = 0; i < mapSize; ++i)
    {
        for (int j = 0; j < mapSize; ++j)
        {
            const float height = std::max(noiseEngine.normalizedOctaveNoise2D(i / float(mapSize), j / float(mapSize), 6) * 0.5f, 0.f);

            Cube cube;

            cube.edgeSize = cubeEdge;
            cube.position.x = (i - mapSize / 2.f) * cubeEdge;
            cube.position.y = (j - mapSize / 2.f) * cubeEdge;

            makeWaterIfNeeded(m_worldBuffer.emplace_back(cube), i);

            for (float currHeight = cubeEdge; currHeight < height; currHeight += cubeEdge)
            {
                cube.position.z = currHeight;
                makeWaterIfNeeded(m_worldBuffer.emplace_back(cube), i);
            }
        }
    }
}
 
EndNamespaceOlympus
