#pragma once

#include <exception>
#include <map>
#include <string_view>
#include <mutex>
#include <functional>
#include <numeric>

#include <boost/circular_buffer.hpp>

#include <glm/glm.hpp>

#include <utils/macros.h>
#include <utils.h>

struct GLFWwindow;

BeginNamespaceOlympus

DeclareInfoException(WindowCreationFailed);
DeclareInfoException(OpenGLDynamicSymbolsBindingError);
DeclareInfoException(ThreadSelectionError);

class OpenGLGLFWContext
{
    OlyNonCopyableMovable(OpenGLGLFWContext)
public:
    using GLFWKeyCallback = std::function<void(int, int, int)>;
    using GLFWKeyPressCallback = std::function<void()>;

    struct InitParameters
    {
        int verMajor{ 0 };
        int verMinor{ 0 };
        std::string windowTitle;
        std::string glslVersion;
        unsigned windowHeight{ 0 };
        unsigned windowWidth{ 0 };
        bool resizable{ false };
    };

    explicit OpenGLGLFWContext(const InitParameters& initParams);
    ~OpenGLGLFWContext();

    bool windowShoudNotClose() const noexcept(true);
    glm::ivec2 getWindowSize() const noexcept(true);

    glm::vec2 getNormalizedWindowPosition(glm::ivec2 pxPosition) const;

    void setThreadContext(bool makeContextCurrent);

    void addKeyboardCallback(int glfwKeyCode, GLFWKeyCallback keyCallback);
    void addKeyboardPressCallback(int glfwKeyCode, GLFWKeyPressCallback keyPressCallback);

    // These methods should be called from the main thread!
    void onFrameStart();
    void onFrameEnd();

    // This methods should be called from the render thread (where OpenGL is initialized)
    void renderFrameStart();
    void renderFrameEnd();

    void addImGuiDebugOutputFunctor(std::function<void()> outputFunctor);

private:
    void ensureMainThread(const char* funcName) const;
    void updateFPS();
    unsigned calculateNormalizedFPS() const;

    void renderDebugInfo();

    bool m_showDebug{ false };

    std::multimap<int, GLFWKeyCallback> m_keysMapping;
    std::vector<std::function<void()>> m_imGUIDebugOutputFunctors;

    GLFWwindow* m_window;
    std::string m_title;

    double m_lastTimeStamp{ 0. };

    // Last N fps values are stored here to avoid spikes and provide consistent value
    boost::circular_buffer<unsigned> m_latestFPS;

    std::function<void(int, int)> onResize;

    std::mutex m_GLFWContextMutex;
};

EndNamespaceOlympus
