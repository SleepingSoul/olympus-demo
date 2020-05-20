#pragma once

BeginNamespaceOlympus

class EngineImpl;
class TextureStorage;
class AsyncVideostreamListener;
class JobSystem;
class OpenGLGLFWContext;

class Engine
{
    OlyNonCopyableMovable(Engine)
public:
    static Engine& instance();

    void initialize();

    bool initializedSuccessfuly() const;

    AsyncVideostreamListener& getAsyncVideostreamListener();
    JobSystem& getJobSystem();
    OpenGLGLFWContext& getWindowContext();

    double getTimeFromStart() const;

    int run();

private:
    Engine() = default;

    std::unique_ptr<EngineImpl> m_impl;
};

EndNamespaceOlympus

#define olyEngine oly::Engine::instance()

