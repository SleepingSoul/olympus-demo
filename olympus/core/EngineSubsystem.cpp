#include <pch.h>
#include "EngineSubsystem.h"
#include <EngineImpl.h>

#include <utils/threading_utils.h>

BeginNamespaceOlympus

EngineSubsystem::EngineSubsystem(EngineImpl& engine)
    : m_engine(engine)
{}

void EngineSubsystem::update()
{
    if (!threading::isMainThread)
    {
        olyError("[EngineSubsystem] Subsystem is being updated from the wrong thread.");
    }
}

EndNamespaceOlympus
