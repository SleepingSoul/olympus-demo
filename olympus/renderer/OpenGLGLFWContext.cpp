#include "OpenGLGLFWContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <easy/profiler.h>
#include <fmt/ostream.h>

#include <extra_std/extra_std.h>
#include <logging/logging.h>
#include <utils/olyerror.h>
#include <utils/threading_utils.h>

BeginNamespaceOlympus

#define EnsureMainThread ensureMainThread(__FUNCTION__)

namespace
{
    void framebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
    }

    void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        EASY_FUNCTION();

        const auto& mapping = *static_cast<std::multimap<int, std::function<void(int, int, int)>>*>(glfwGetWindowUserPointer(window));

        auto [cbBegin, cbEnd] = mapping.equal_range(key);

        while (cbBegin != cbEnd)
        {
            std::invoke(cbBegin->second, scancode, action, mode);
            ++cbBegin;
        }
    }
}

OpenGLGLFWContext::OpenGLGLFWContext(const InitParameters& initParams)
    : m_title(initParams.windowTitle)
    , m_latestFPS(60)
{
    // GLFW is required to be initialized only in main thread

    if (!threading::isMainThread())
    {
        olyError("Attempt to initialize OpenGLGLFWContext in secondary thread");

        throw ThreadSelectionError("Not main thread");
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, initParams.verMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, initParams.verMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, initParams.resizable ? GLFW_TRUE : GLFW_FALSE);

    if (m_window = glfwCreateWindow(initParams.windowWidth, initParams.windowHeight, initParams.windowTitle.c_str(), NULL, NULL);
        m_window == nullptr)
    {
        const char* description;
        const int errorCode = glfwGetError(&description);
        throw WindowCreationFailed(estd::format("GLFW window creation failed. GLFW Error code: %d: %s", errorCode, description));
    }

    glfwMakeContextCurrent(m_window);

    const int dllLoadResult = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    if (!dllLoadResult)
    {
        throw OpenGLDynamicSymbolsBindingError(estd::format("GLAD load error. Code: %d", dllLoadResult));
    }

    glfwSetFramebufferSizeCallback(m_window, &framebufferSizeCallback);
    glfwSetKeyCallback(m_window, &keyboardCallback);

    glfwSetWindowUserPointer(m_window, &m_keysMapping);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(initParams.glslVersion.c_str());

    addKeyboardPressCallback(GLFW_KEY_F1, [this]
    {
        m_showDebug = !m_showDebug;
    });

    addKeyboardPressCallback(GLFW_KEY_ESCAPE, [this]
    {
        logging::info("ESC press was detected.");

        glfwSetWindowShouldClose(m_window, true);
    });
}

OpenGLGLFWContext::~OpenGLGLFWContext()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

bool OpenGLGLFWContext::windowShoudNotClose() const noexcept(true)
{
    return !glfwWindowShouldClose(m_window);
}

glm::ivec2 OpenGLGLFWContext::getWindowSize() const noexcept(true)
{
    EnsureMainThread;

    glm::ivec2 result;

    glfwGetWindowSize(m_window, &result.x, &result.y);

    return result;
}

void OpenGLGLFWContext::setThreadContext(bool makeContextCurrent)
{
    EASY_FUNCTION("Make context current: %d", makeContextCurrent);

    std::lock_guard lg(m_GLFWContextMutex);

    const GLFWwindow* const currContext = glfwGetCurrentContext();

    static std::thread::id lastCallerThreadID;
    const auto noThreadID = std::thread::id{};

    const auto currentThreadID = std::this_thread::get_id();

    if (lastCallerThreadID != noThreadID && currentThreadID != lastCallerThreadID &&
        makeContextCurrent && currContext)
    {
        olyError("Attempt to set positive context in thread: {} when it wasn't unset in the previous thread: {}", currentThreadID, lastCallerThreadID);
        return;
    }
    else
    {
        lastCallerThreadID = currentThreadID;
    }

    if (makeContextCurrent && currContext == nullptr)
    {
        glfwMakeContextCurrent(m_window);
    }
    else if (!makeContextCurrent && currContext == m_window)
    {
        glfwMakeContextCurrent(nullptr);
    }
}

void OpenGLGLFWContext::addKeyboardCallback(int glfwKeyCode, GLFWKeyCallback keyCallback)
{
    EnsureMainThread;

    m_keysMapping.emplace(glfwKeyCode, std::move(keyCallback));
}

void OpenGLGLFWContext::addKeyboardPressCallback(int glfwKeyCode, GLFWKeyPressCallback keyPressCallback)
{
    addKeyboardCallback(glfwKeyCode, [onPress = std::move(keyPressCallback)](int scancode, int action, int mode)
    {
        if (action == GLFW_PRESS)
        {
            onPress();
        }
    });
}

void OpenGLGLFWContext::ensureMainThread(const char* funcName) const
{
    if (!threading::isMainThread())
    {
        olyError("Function '{}' should be called only from the main thread, but called from the other.", funcName);
    }
}

void OpenGLGLFWContext::updateFPS()
{
    const double frameEndTimeStamp = glfwGetTime();
    m_latestFPS.push_back(static_cast<unsigned>(1. / (frameEndTimeStamp - m_lastTimeStamp)));
    m_lastTimeStamp = frameEndTimeStamp;
}

unsigned OpenGLGLFWContext::calculateNormalizedFPS() const
{
    if (m_latestFPS.empty())
    {
        return 0;
    }
    return std::accumulate(m_latestFPS.begin(), m_latestFPS.end(), 0u) / static_cast<unsigned>(m_latestFPS.size());
}

void OpenGLGLFWContext::renderDebugInfo()
{
    if (!m_showDebug)
    {
        return;
    }

    ImGui::Begin("Main window debug");

    ImGui::Text("Real current FPS: %d", m_latestFPS.back());
    ImGui::Text("Normalized current FPS: %d", calculateNormalizedFPS());

    ImGui::End();
}

void OpenGLGLFWContext::onFrameStart()
{
    EnsureMainThread;

    EASY_BLOCK("ImGui opengl new frame", profiler::colors::Red);

    setThreadContext(true);

    ImGui_ImplOpenGL3_NewFrame();

    setThreadContext(false);

    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void OpenGLGLFWContext::onFrameEnd()
{
    EnsureMainThread;

    EASY_BLOCK("GLFW poll events");
    glfwPollEvents();
    EASY_END_BLOCK;

    updateFPS();
}

void OpenGLGLFWContext::renderFrameStart()
{
    EASY_FUNCTION();

    setThreadContext(true);
}

void OpenGLGLFWContext::renderFrameEnd()
{
    renderDebugInfo();

    EASY_BLOCK("ImGUI prosessing");
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    EASY_END_BLOCK;

    EASY_BLOCK("GLFW swap buffers");
    glfwSwapBuffers(m_window);
    EASY_END_BLOCK;

    setThreadContext(false);
}

#undef EnsureMainThread

EndNamespaceOlympus
