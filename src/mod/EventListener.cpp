#include "Global.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/event/player/PlayerConnectEvent.h"
#include "ll/api/event/player/PlayerJoinEvent.h"
#include "ll/api/event/player/PlayerLeaveEvent.h"

using namespace ll::event;

#define EVENT_FUNCTION(event) void m## event(event& ev)
#define EVENT_REGISTER(event) bus.emplaceListener<event>(&m## event)
#define EVENT_REGISTER_PRIORITY(event, priority) bus.emplaceListener<event>(&m## event, EventPriority::##priority)
#define GET_SP()                                                                                                   \
    ServerPlayer* sp = &ev.self();                                                                                     \
    if (sp->isSimulatedPlayer()) return;

namespace PlayerRegister {

EVENT_FUNCTION(PlayerConnectEvent) {
    GET_SP();
    LOGGER.info("connect {}", sp->getRealName());
    if(!sp->getXuid().empty()) return;

}

EVENT_FUNCTION(PlayerJoinEvent) {

}

EVENT_FUNCTION(PlayerLeaveEvent) {

}


bool registerEvents() {
    auto& bus = EventBus::getInstance();
    EVENT_REGISTER(PlayerConnectEvent);
    EVENT_REGISTER(PlayerJoinEvent);
    EVENT_REGISTER(PlayerLeaveEvent);
    return true;
}

} // namespace OreShop
