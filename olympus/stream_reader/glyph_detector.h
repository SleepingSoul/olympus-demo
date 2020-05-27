#pragma once

#include <optional>

#include <glm/glm.hpp>

#include <utils/macros.h>

#include <GlyphOptions.h>

BeginNamespaceOlympus

namespace glyph
{
    std::optional<glm::mat4> detectGlyph(const GlyphRecognitionOptions& options, cv::Mat frame);
}

EndNamespaceOlympus
