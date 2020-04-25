#include "OpenGLGLFWContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <extra_std/extra_std.h>


namespace
{
    void framebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        const auto& mapping = *static_cast<std::multimap<int, std::function<void(int, int, int)>>*>(glfwGetWindowUserPointer(window));

        auto [cbBegin, cbEnd] = mapping.equal_range(key);

        while (cbBegin != cbEnd)
        {
            std::invoke(cbBegin->second, scancode, action, mode);
            ++cbBegin;
        }
    }
}

oly::OpenGLGLFWContext::OpenGLGLFWContext(const InitParameters& initParams)
    : m_title(initParams.windowTitle)
    , m_options()
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

oly::OpenGLGLFWContext::~OpenGLGLFWContext()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

bool oly::OpenGLGLFWContext::windowShoudNotClose() const noexcept(true)
{
    return !glfwWindowShouldClose(m_window);
}

glm::ivec2 oly::OpenGLGLFWContext::getWindowSize() const noexcept(true)
{
    glm::ivec2 result;

    glfwGetWindowSize(m_window, &result.x, &result.y);

    return result;
}

void oly::OpenGLGLFWContext::addKeyboardCallback(int glfwKeyCode, GLFWKeyCallback keyCallback)
{
    m_keysMapping.emplace(glfwKeyCode, std::move(keyCallback));
}

void oly::OpenGLGLFWContext::onFrameStart()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void oly::OpenGLGLFWContext::onFrameEnd()
{
    const double frameEndTimeStamp = glfwGetTime();
    m_FPS = static_cast<unsigned>(1. / (frameEndTimeStamp - m_lastTimeStamp));
    m_lastTimeStamp = frameEndTimeStamp;

    if (m_options.showFPS)
    {
        ImGui::Text("FPS: %u", m_FPS);
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}
