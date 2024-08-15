#include "Event/engineevents.h"

namespace EngineEvents {
    Event<const std::string&, LuaMessageType> LuaMessageEvent;
}