#include <pch.h>
#include"BaseRecognitionSubsystem.h"

#include <opencv2/opencv.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <EngineImpl.h>

BeginNamespaceOlympus

namespace
{
    glm::mat4 convertMatrix(cv::Mat matrix)
    {
        auto type = matrix.type();
        auto needed = CV_32FC1;

        if (matrix.cols != 3 || matrix.rows != 3)
        {
            olyError("[BaseRecognitionSubsystem] Failed to convert matrices.");
            return glm::identity<glm::mat4>();
        }

        if (matrix.type() != CV_32FC1)
        {
            matrix.convertTo(matrix, CV_32FC1);
        }

        glm::mat3 result;
        std::memcpy(glm::value_ptr(result), matrix.ptr(), 9 * sizeof(float));
        //result = glm::transpose(result);

        return glm::mat4(result);
    }
}

BaseRecognitionSubsystem::BaseRecognitionSubsystem(EngineImpl& engine)
    : Base(engine)
    , m_glyphOptions("data/calibration.data")
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
        jobSystem.addJob(std::make_unique<RecognizeBaseJob>(listener.getLatestFrame(), m_glyphOptions, [this](std::optional<glm::mat4>&& result)
        {
            std::lock_guard lg(m_mutex);

            m_viewMatrix = std::move(result).value_or(glm::identity<glm::mat4>());

            m_isRecognizing.store(false);
        }));
    }

    auto& windowContext = m_engine.getWindowContext();
    auto& renderer = m_engine.getRenderer();

    Cube cube;
    cube.edgeSize = 5;
    cube.position = { 0, 0, 0 };
    cube.face = m_engine.getTextureStorage().getTexture(TextureID::Crate);

    renderer.getCubeRenderComponent().renderCubes(std::move(cube));

    std::lock_guard lg(m_mutex);
    renderer.getCamera().setTransformMatrix(m_viewMatrix);
}

EndNamespaceOlympus
