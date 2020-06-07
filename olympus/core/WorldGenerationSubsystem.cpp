#include <pch.h>
#include "WorldGenerationSubsystem.h"

#include <PerlinNoise/PerlinNoise.hpp>

#include <utils/math.h>

#include <EngineImpl.h>

BeginNamespaceOlympus

WorldGenerationSubsystem::WorldGenerationSubsystem(EngineImpl& engine)
    : Base(engine)
{
    constexpr size_t mapSize = 30;
    constexpr float cubeEdge = 0.01f;

    auto noiseEngine = siv::BasicPerlinNoise<float>();

    std::array<std::array<float, mapSize>, mapSize> heightMap;

    for (int i = 0; i < mapSize; ++i)
    {
        for (int j = 0; j < mapSize; ++j)
        {
            heightMap[i][j] = noiseEngine.normalizedOctaveNoise2D(i / float(mapSize), j / float(mapSize), 6) * 0.5f;

            if (heightMap[i][j] < 0.f)
            {
                heightMap[i][j] = 0.f;
            }
        }
    }

    for (int i = 0; i < mapSize; ++i)
    {
        for (int j = 0; j < mapSize; ++j)
        {
            Cube cube;

            cube.edgeSize = cubeEdge;
            cube.face = m_engine.getTextureStorage().getTexture(TextureID::Crate);
            cube.position.x = (i - mapSize / 2.f) * cubeEdge;
            cube.position.y = (j - mapSize / 2.f) * cubeEdge;

            m_world.emplace_back(cube);

            for (float height = cubeEdge; height < heightMap[i][j]; height += cubeEdge)
            {
                cube.position.z = height;
                m_world.emplace_back(cube);
            }
        }
    }
}

void WorldGenerationSubsystem::update()
{
    Base::update();

    auto& renderer = m_engine.getRenderer();

    const auto& markers = m_engine.getMarkers();

    CubeRenderComponent::CubeWorld cubeWorld;

    for (const auto& marker : markers)
    {
        cubeWorld.modelViewMatrix = marker.modelviewMatrix.clone();
        cubeWorld.projectionMatrix = marker.projectionMatrix.clone();
        cubeWorld.cubes = m_world;

        renderer.getCubeRenderComponent().renderCubes(std::move(cubeWorld));
    }
}

EndNamespaceOlympus
