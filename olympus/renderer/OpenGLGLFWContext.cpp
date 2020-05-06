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

BeginNamespaceOlympus

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
    , m_options()
    , m_latestFPS(60)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, initParams.verMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, initParams.verMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    addKeyboardCallback(GLFW_KEY_F1, [this](int scancode, int action, int mode)
    {
        if (action == GLFW_PRESS)
        {
            m_options.showFPS = !m_options.showFPS;
        }
    });

    addKeyboardCallback(GLFW_KEY_ESCAPE, [this](int scancode, int action, int mode)
    {
        if (action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(m_window, true);
        }
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
    glm::ivec2 result;

    glfwGetWindowSize(m_window, &result.x, &result.y);

    return result;
}

void OpenGLGLFWContext::setThreadContext(bool makeContextCurrent)
{
    EASY_FUNCTION("Make context current: %d", makeContextCurrent);

    std::lock_guard lg(m_mutex);

    const GLFWwindow* const currContext = glfwGetCurrentContext();

    if (makeContextCurrent && currContext == nullptr)
    {
        logging::debug("GLFW context set to window: {} for thread: {}", reinterpret_cast<void*>(m_window), std::this_thread::get_id());
        glfwMakeContextCurrent(m_window);
    }
    else if (!makeContextCurrent && currContext == m_window)
    {
        logging::debug("GLFW context set to nullptr from thread: {}", std::this_thread::get_id());
        glfwMakeContextCurrent(nullptr);
    }
}

void OpenGLGLFWContext::addKeyboardCallback(int glfwKeyCode, GLFWKeyCallback keyCallback)
{
    m_keysMapping.emplace(glfwKeyCode, std::move(keyCallback));
}

void OpenGLGLFWContext::onFrameStart()
{
}

void OpenGLGLFWContext::updateFPS()
{
    const double frameEndTimeStamp = glfwGetTime();
    m_latestFPS.push_back(static_cast<unsigned>(1. / (frameEndTimeStamp - m_lastTimeStamp)));
    m_lastTimeStamp = frameEndTimeStamp;
}

void OpenGLGLFWContext::onFrameEnd()
{
    EASY_BLOCK("GLFW poll events");
    glfwPollEvents();
    EASY_END_BLOCK;

    updateFPS();
}

void OpenGLGLFWContext::renderFrameStart()
{
    EASY_FUNCTION();

    setThreadContext(true);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void OpenGLGLFWContext::renderFrameEnd()
{
    if (m_options.showFPS)
    {
        ImGui::Text("FPS: %u", calculateNormalizedFPS());
    }

    EASY_BLOCK("ImGUI prosessing");
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    EASY_END_BLOCK;

    EASY_BLOCK("GLFW swap buffers");
    glfwSwapBuffers(m_window);
    EASY_END_BLOCK;
}

EndNamespaceOlympus
