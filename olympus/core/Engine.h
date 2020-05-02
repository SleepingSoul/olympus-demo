#pragma once

BeginNamespaceOlympus

class EngineImpl;

class Engine
{
    OlyNonCopyableMovable(Engine)
public:
    static Engine& instance();

    void initialize();

    bool initializedSuccessfuly() const;

    int run();

private:
    Engine() = default;

    std::unique_ptr<EngineImpl> m_impl;
};

EndNamespaceOlympus

#define olyEngine oly::Engine::instance()

