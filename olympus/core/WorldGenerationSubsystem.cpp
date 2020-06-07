#include <pch.h>
#include "WorldGenerationSubsystem.h"

#include <OpenGLRenderer.h>

#include <EngineImpl.h>

BeginNamespaceOlympus

WorldGenerationSubsystem::WorldGenerationSubsystem(EngineImpl& engine)
    : Base(engine)
{
}

void WorldGenerationSubsystem::update()
{
    auto& renderer = m_engine.getRenderer();

    Cube cube;
    cube.edgeSize = 0.1f;
    cube.position = { 0, 0, 0 };
    cube.face = m_engine.getTextureStorage().getTexture(TextureID::Crate);

    std::vector<Cube> cubes(6, cube);

    cubes[1].position.x = 0.1f;
    cubes[2].position.x = -0.1f;
    cubes[3].position.y = 0.1f;
    cubes[4].position.y = -0.1f;
    cubes[5].position.x = 0.2f;

    renderer.getCubeRenderComponent().renderCubes(std::move(cubes));
}

EndNamespaceOlympus
