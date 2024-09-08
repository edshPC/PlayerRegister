#include "mod/PlayerRegisterMod.h"

#include <memory>

#include "ll/api/mod/RegisterHelper.h"

namespace PlayerRegister {

static std::unique_ptr<PlayerRegisterMod> instance;

PlayerRegisterMod& PlayerRegisterMod::getInstance() { return *instance; }

bool PlayerRegisterMod::load() {
    getSelf().getLogger().debug("Loading...");
    // Code for loading the mod goes here.
    return true;
}

bool PlayerRegisterMod::enable() {
    getSelf().getLogger().debug("Enabling...");
    // Code for enabling the mod goes here.
    return true;
}

bool PlayerRegisterMod::disable() {
    getSelf().getLogger().debug("Disabling...");
    // Code for disabling the mod goes here.
    return true;
}

} // namespace my_mod

LL_REGISTER_MOD(PlayerRegister::PlayerRegisterMod, PlayerRegister::instance);
