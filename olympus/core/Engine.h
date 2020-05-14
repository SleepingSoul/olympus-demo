#pragma once

BeginNamespaceOlympus

class EngineImpl;
class TextureStorage;
class AsyncVideostreamListener;
class JobSystem;

class Engine
{
    OlyNonCopyableMovable(Engine)
public:
    static Engine& instance();

    void initialize();

    bool initializedSuccessfuly() const;

    AsyncVideostreamListener& getAsyncVideostreamListener();
    JobSystem& getJobSystem();

    int run();

private:
    Engine() = default;

    std::unique_ptr<EngineImpl> m_impl;
};

EndNamespaceOlympus

#define olyEngine oly::Engine::instance()

