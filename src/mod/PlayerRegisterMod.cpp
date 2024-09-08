#include "mod/PlayerRegisterMod.h"

#include <memory>

#include "ll/api/mod/RegisterHelper.h"

#include "Database.h"

namespace PlayerRegister {

static std::unique_ptr<PlayerRegisterMod> instance;

PlayerRegisterMod& PlayerRegisterMod::getInstance() { return *instance; }

bool PlayerRegisterMod::load() {
    return Database::init() && setupHooks() && registerEvents();
}

bool PlayerRegisterMod::enable() {
    PlayerManager::setFakeUUID("player_server_61e12c79-09f6-4cd1-b752-6f7cd8d25ea5", "player_server_66612c79-09f6-4cd1-b752-6f7cd8d25ea5");
    return true;
}

bool PlayerRegisterMod::disable() {
    return true;
}

} // namespace my_mod

LL_REGISTER_MOD(PlayerRegister::PlayerRegisterMod, PlayerRegister::instance);
