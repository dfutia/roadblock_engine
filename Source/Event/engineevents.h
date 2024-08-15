#pragma once
#ifndef ENGINE_EVENTS_H
#define ENGINE_EVENTS_H

#include "Event/event.h"

#include <string>

enum class LuaMessageType {
    Info,
    Warning,
    Error
};

namespace EngineEvents {
    extern  Event<const std::string&, LuaMessageType> LuaMessageEvent;
}

#endif 