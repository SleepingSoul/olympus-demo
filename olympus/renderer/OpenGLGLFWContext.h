#pragma once

#include <exception>
#include <map>
#include <string_view>
#include <mutex>
#include <functional>
#include <glm/glm.hpp>
#include <utils/macros.h>
#include <utils.h>

struct GLFWwindow;

namespace oly
{
    DeclareInfoException(WindowCreationFailed);
    DeclareInfoException(OpenGLDynamicSymbolsBindingError);

    class OpenGLGLFWContext
    {
        OlyNonCopyableMovable(OpenGLGLFWContext)
    public:
        using GLFWKeyCallback = std::function<void(int, int, int)>;

        struct InitParameters
        {
            int verMajor{ 0 };
            int verMinor{ 0 };
            std::string windowTitle;
            std::string glslVersion;
            unsigned windowHeight{ 0 };
            unsigned windowWidth{ 0 };
        };

        explicit OpenGLGLFWContext(const InitParameters& initParams);
        ~OpenGLGLFWContext();

        bool windowShoudNotClose() const noexcept(true);
        glm::ivec2 getWindowSize() const noexcept(true);

        void addKeyboardCallback(int glfwKeyCode, GLFWKeyCallback keyCallback);

        void onFrameStart();
        void onFrameEnd();

    private:
        struct Options
        {
            bool showFPS;
        };

        Options m_options;

        std::multimap<int, GLFWKeyCallback> m_keysMapping;

        GLFWwindow* m_window;
        std::string m_title;
        double m_lastTimeStamp{ 0. };
        unsigned m_FPS{ 0 };
        std::function<void(int, int)> onResize;
    };
}
