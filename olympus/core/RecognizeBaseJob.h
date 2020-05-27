#pragma once

#include <glm/glm.hpp>

#include <GlyphOptions.h>
#include <StreamDecoder.h>

#include <Job.h>

BeginNamespaceOlympus

class RecognizeBaseJob : public Job
{
public:
    using Base = Job;
    using ThreadSafeOutputFunctor = std::function<void(std::optional<glm::mat4>&&)>;

    RecognizeBaseJob(cv::Mat&& frame, const glyph::GlyphRecognitionOptions& options, ThreadSafeOutputFunctor outputFunctor);

    void execute() override;

private:
    cv::Mat m_frame;
    const glyph::GlyphRecognitionOptions& m_options;

    ThreadSafeOutputFunctor m_outputFunctor;
};

EndNamespaceOlympus
