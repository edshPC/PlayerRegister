#include "mod/PlayerRegisterMod.h"

#include "Command.h"

#include <memory>

#include "ll/api/i18n/I18n.h"
#include "ll/api/mod/RegisterHelper.h"

#include "Config.h"
#include "manager/Database.h"

namespace PlayerRegister {

static std::unique_ptr<ll::i18n::I18n> locale;

PlayerRegisterMod& PlayerRegisterMod::getInstance() {
    static PlayerRegisterMod instance;
    return instance;
}

std::string PlayerRegisterMod::tr(const std::string& key) {
    return std::string(locale->get(key, CONF.lang));
}

bool PlayerRegisterMod::load() {
    if (!Config::init()) return false;
    locale = std::make_unique<ll::i18n::I18n>();
    auto exp = locale->load(NATIVE_MOD.getLangDir());
    return exp.has_value() && Database::init() && setupHooks();
}

bool PlayerRegisterMod::enable() { return Command::init(); }

bool PlayerRegisterMod::disable() { return true; }

} // namespace PlayerRegister

LL_REGISTER_MOD(PlayerRegister::PlayerRegisterMod, PlayerRegister::PlayerRegisterMod::getInstance());
