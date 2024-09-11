#include "Global.h"
#include "PlayerManager.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/event/player/PlayerLeaveEvent.h"

using namespace ll::event;

namespace PlayerRegister {

bool registerEvents() {
    auto& bus = EventBus::getInstance();
    bus.emplaceListener<PlayerLeaveEvent>([](PlayerLeaveEvent& ev) {
        ServerPlayer* sp = &ev.self();                                                                                     \
        if (sp->isSimulatedPlayer()) return;
        PlayerManager::unloadPlayer(sp);
    });
    return true;
}

} // namespace PlayerRegister
