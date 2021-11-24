#pragma once

#include <opencv2/core/mat.hpp>

#include <utils/macros.h>
#include <utils/forward.h>

#include <containers/DoubleBuffer.h>

#include <ShaderProgram.h>
#include <Camera.h>
#include <Model.h>

BeginNamespaceOlympus

class AnyModelRendererComponent
{
public:
    struct WorldModel
    {
        Model* model;
        glm::vec3 position{ 0.f, 0.f, 0.f };
        glm::vec3 rotationAxis{ 1.f, 0.f, 0.f };
        glm::vec3 scale{ 1.f, 1.f, 1.f };
        float rotationValueRad{ 0.f };
        cv::Mat modelView;
        cv::Mat projection;
    };

    AnyModelRendererComponent();

    OlyPerfectForwardInContainer(renderModels, m_models.frontBuffer())

    void render(const Camera& camera);

    void swapBuffers() { m_models.swapBuffersSafe(); }

    void clearBackBuffer() { m_models.clearBackBuffer(); }

private:
    ShaderProgram m_modelShader;

    DoubleBuffer<std::vector<WorldModel>> m_models;
};

EndNamespaceOlympus