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
    return true;
}

bool PlayerRegisterMod::disable() {
    return true;
}

} // namespace my_mod

LL_REGISTER_MOD(PlayerRegister::PlayerRegisterMod, PlayerRegister::instance);
