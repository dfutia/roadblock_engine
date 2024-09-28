#pragma once
#ifndef ENGINE_EVENTS_H
#define ENGINE_EVENTS_H

#include "Event/event.h"
#include "Scene/Nodes/script.h"

#include <string>

enum class LuaMessageType {
    Info,
    Warning,
    Error
};

namespace EngineEvents {
    extern Event<const std::string&, LuaMessageType> LuaMessageEvent;
    extern Event<Script&> OpenScriptEvent;
}

#endif 