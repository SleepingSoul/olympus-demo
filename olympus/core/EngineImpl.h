#pragma once

#include <memory>
#include <optional>

#include <OpenGLGLFWContext.h>
#include <OpenGLVoxelRenderer.h>
#include <utils/macros.h>

namespace oly
{
    class EngineImpl
    {
        OlyNonCopyableMovable(EngineImpl)
    public:
        EngineImpl();

        bool initializedSuccessfuly() const { return m_successfulInitialization; }

        int run();

    private:
        std::optional<std::string> m_profilerFile;

        std::unique_ptr<OpenGLGLFWContext> m_openGLGLFWContext;
        std::unique_ptr<OpenGLVoxelRenderer> m_openGLVoxelRenderer;

        bool m_successfulInitialization{ false };
    };
}
