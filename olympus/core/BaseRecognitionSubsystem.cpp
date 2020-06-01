#include <pch.h>
#include"BaseRecognitionSubsystem.h"

#include <opencv2/opencv.hpp>

#include <managers/CommandLineManager.h>

#include <EngineImpl.h>

BeginNamespaceOlympus

BaseRecognitionSubsystem::BaseRecognitionSubsystem(EngineImpl& engine)
    : Base(engine)
    , m_markerOptions("data/calibration3.yml",
        cv::Size(*olyCommandLineManager.getLong(CommandLineOptions::Width), *olyCommandLineManager.getLong(CommandLineOptions::Height)))
{}

void BaseRecognitionSubsystem::update()
{
    Base::update();

    auto& listener = m_engine.getAsyncVideostreamListener();
    auto& jobSystem = m_engine.getJobSystem();

    if (const auto latestFrameID = listener.getLatestFrameID(); !m_isRecognizing.load() && latestFrameID != m_lastFrameID)
    {
        m_lastFrameID = latestFrameID;
        m_isRecognizing.store(true);
        jobSystem.addJob(std::make_unique<RecognizeBaseJob>(listener.getLatestFrame(), m_markerOptions, [this](std::optional <markers::DetectResult>&& result)
        {
            std::lock_guard lg(m_mutex);

            if (result)
            {
                m_detectResult = std::move(*result);
            }

            m_isRecognizing.store(false);
        }));
    }

    auto& windowContext = m_engine.getWindowContext();
    auto& renderer = m_engine.getRenderer();

    Cube cube;
    cube.edgeSize = 0.1f;
    cube.position = { 0, 0, 0 };
    cube.face = m_engine.getTextureStorage().getTexture(TextureID::Crate);

    std::vector<Cube> cubes(6, cube);

    cubes[1].position.y = 0.1f;
    cubes[2].position.y = -0.1f;
    cubes[3].position.z = 0.1f;
    cubes[4].position.z = -0.1f;
    cubes[5].position.x = 0.3f;

    renderer.getCubeRenderComponent().renderCubes(std::move(cubes));

    std::lock_guard lg(m_mutex);
    renderer.getCamera().setARModelViewMatrix(m_detectResult.modelviewMatrix);
    renderer.getCamera().setARProjectionMatrix(m_detectResult.projectionMatrix);
    renderer.getCamera().setCameraMatrix(m_markerOptions.getParameters().CameraMatrix);
    renderer.getCamera().setDistortionMatrix(m_markerOptions.getParameters().Distorsion);
}

EndNamespaceOlympus
