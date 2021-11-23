#pragma once

#include <opencv2/core/mat.hpp>

#include <utils/macros.h>

#include <containers/DoubleBuffer.h>

#include <ShaderProgram.h>
#include <Camera.h>
#include <Model.h>

BeginNamespaceOlympus

class AnyModelRendererComponent
{
public:
    AnyModelRendererComponent();

    void render(const Camera& camera);

    void setProjection(const cv::Mat& projection) { m_projection = projection; }
    void setModelview(const cv::Mat& modelView) { m_modelView = modelView; }

private:
    ShaderProgram m_modelShader;
    Model m_model;

    cv::Mat m_projection;
    cv::Mat m_modelView;
};

EndNamespaceOlympus