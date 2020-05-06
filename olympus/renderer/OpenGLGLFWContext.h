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

    void setThreadContext(bool makeContextCurrent);

    void addKeyboardCallback(int glfwKeyCode, GLFWKeyCallback keyCallback);

    // These methods should be called from the main thread!
    void onFrameStart();
    void onFrameEnd();

    // This methods should be called from the render thread (where OpenGL is initialized)
    void renderFrameStart();
    void renderFrameEnd();

private:
    void updateFPS();
    unsigned calculateNormalizedFPS() const { return std::accumulate(m_latestFPS.begin(), m_latestFPS.end(), 0u) / static_cast<unsigned>(m_latestFPS.size()); }

    struct Options
    {
        bool showFPS;
    };

    Options m_options;

    std::multimap<int, GLFWKeyCallback> m_keysMapping;

    GLFWwindow* m_window;
    std::string m_title;

    double m_lastTimeStamp{ 0. };

    // Last N fps values are stored here to avoid spikes and provide consistent value
    boost::circular_buffer<unsigned> m_latestFPS;

    std::function<void(int, int)> onResize;

    std::mutex m_mutex;
};

EndNamespaceOlympus
