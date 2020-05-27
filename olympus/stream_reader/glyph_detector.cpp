#include "glyph_detector.h"

#include <array>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <logging/logging.h>

BeginNamespaceOlympus

namespace glyph
{
    namespace
    {
        const glm::mat4 InverseMatrix = {
             1.f,  1.f,  1.f,  1.f,
            -1.f, -1.f, -1.f, -1.f,
            -1.f, -1.f, -1.f, -1.f,
             1.f,  1.f,  1.f,  1.f
        };
    }

    std::optional<glm::mat4> detectGlyph(const GlyphRecognitionOptions& options, cv::Mat frame)
    {
        cv::cvtColor(frame, frame, cv::ColorConversionCodes::COLOR_BGR2GRAY);

        cv::Mat corners;

        const bool cornersFound = cv::findChessboardCorners(frame, options.getSize(), corners);

        if (!cornersFound)
        {
            return std::nullopt;
        }

        cv::cornerSubPix(frame, corners, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::Type::MAX_ITER, 30, 0.001));

        if (corners.empty())
        {
            return std::nullopt;
        }

        logging::info("Wow! Chessboard corners found!");

        cv::Mat rvecs, tvecs;

        const bool pnpSolved = cv::solvePnPRansac(options.getObjPoints(), corners, options.getCameraMatrix(), options.getDistortionMatrix(),
            rvecs, tvecs);

        if (!pnpSolved)
        {
            return std::nullopt;
        }

        cv::Mat rotation, viewMatrix(4, 4, CV_64F);

        cv::Rodrigues(rvecs, rotation);

        for (unsigned int row = 0; row < 3; ++row)
        {
            for (unsigned int col = 0; col < 3; ++col)
            {
                viewMatrix.at<double>(row, col) = rotation.at<double>(row, col);
            }
            viewMatrix.at<double>(row, 3) = tvecs.at<double>(row, 0);
        }
        viewMatrix.at<double>(3, 3) = 1.0f;

        cv::Mat cvToGl = cv::Mat::zeros(4, 4, CV_64F);
        cvToGl.at<double>(0, 0) = 1.0f;
        cvToGl.at<double>(1, 1) = -1.0f; // Invert the y axis
        cvToGl.at<double>(2, 2) = -1.0f; // invert the z axis
        cvToGl.at<double>(3, 3) = 1.0f;

        viewMatrix = cvToGl * viewMatrix;

        cv::Mat glViewMatrix = cv::Mat::zeros(4, 4, CV_64F);
        cv::transpose(viewMatrix, glViewMatrix);
        glViewMatrix.convertTo(glViewMatrix, CV_32F);

        glm::mat4 mat;
        std::memcpy(glm::value_ptr(mat), &glViewMatrix.at<float>(0, 0), sizeof(float) * 4 * 4);
        return mat;

        /*tvecs.convertTo(tvecs, CV_32F);

        cv::Mat cvViewMatrix;

        cv::Rodrigues(rvecs, cvViewMatrix);

        cvViewMatrix.convertTo(cvViewMatrix, CV_32F);

        glm::mat4 viewMatrix = {
            cvViewMatrix.at<float>(0, 0), cvViewMatrix.at<float>(0, 1), cvViewMatrix.at<float>(0, 2), tvecs.at<float>(0),
            cvViewMatrix.at<float>(1, 0), cvViewMatrix.at<float>(1, 1), cvViewMatrix.at<float>(1, 2), tvecs.at<float>(1),
            cvViewMatrix.at<float>(2, 0), cvViewMatrix.at<float>(2, 1), cvViewMatrix.at<float>(2, 2), tvecs.at<float>(2),
            0.f,                          0.f,                          0.f,                          1.f
        };

        viewMatrix = glm::transpose(viewMatrix * InverseMatrix);

        std::stringstream text;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                text << viewMatrix[i][j] << ' ';
            }
        }

        logging::debug(text.str().c_str());

        text.str({});

        auto r = glm::lookAt(glm::vec3(0.f, 5.f, 5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                text << r[i][j] << ' ';
            }
        }

        logging::debug(text.str().c_str());

        return r;*/
    }
}

EndNamespaceOlympus