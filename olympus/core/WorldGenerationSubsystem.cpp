#include <pch.h>
#include "WorldGenerationSubsystem.h"

#include <PerlinNoise/PerlinNoise.hpp>

#include <utils/math.h>

#include <EngineImpl.h>

BeginNamespaceOlympus

WorldGenerationSubsystem::WorldGenerationSubsystem(EngineImpl& engine)
    : Base(engine)
{}

void WorldGenerationSubsystem::update()
{
    Base::update();

    auto& renderer = m_engine.getRenderer();

    const auto& markers = m_engine.getMarkers();

    for (const auto& marker : markers)
    {
        CubeRenderComponent::CubeWorld cubeWorld;

        cubeWorld.modelViewMatrix = marker.modelviewMatrix.clone();
        cubeWorld.projectionMatrix = marker.projectionMatrix.clone();
        cubeWorld.cubes = generateWorld(static_cast<std::uint32_t>(marker.markerID));

        renderer.getCubeRenderComponent().renderCubes(std::move(cubeWorld));
    }
}

std::vector<Cube> WorldGenerationSubsystem::generateWorld(std::uint32_t seed)
{
    std::vector<Cube> world;

    constexpr size_t mapSize = 30;
    constexpr float cubeEdge = 0.01f;

    world.reserve(mapSize * mapSize);

    auto noiseEngine = siv::BasicPerlinNoise<float>(seed);

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
            cube.position.x = (i - mapSize / 2.f) * cubeEdge;
            cube.position.y = (j - mapSize / 2.f) * cubeEdge;

            world.emplace_back(cube);

            for (float height = cubeEdge; height < heightMap[i][j]; height += cubeEdge)
            {
                cube.position.z = height;
                world.emplace_back(cube);
            }
        }
    }

    const auto time = m_engine.getTimeFromStart();

    int i = 0;
    for (auto& cube : world)
    {
        if (equalsWithAlpha(cube.position.z, 0.f, 0.001f))
        {
            cube.face = m_engine.getTextureStorage().getTexture(TextureID::Water);
            cube.position.z += cubeEdge * static_cast<float>(std::sin((time + i++) * 2)) / 2.f;
            cube.diffuse = glm::vec3(0.1f);
        }
        else
        {
            cube.face = m_engine.getTextureStorage().getTexture(TextureID::Crate);
        }
    }

    return world;
}
 
EndNamespaceOlympus
