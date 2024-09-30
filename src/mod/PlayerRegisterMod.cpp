#include "mod/PlayerRegisterMod.h"

#include "Command.h"

#include <memory>

#include "ll/api/i18n/I18n.h"
#include "ll/api/mod/RegisterHelper.h"

#include "Config.h"
#include "manager/Database.h"

namespace PlayerRegister {

static std::unique_ptr<PlayerRegisterMod> instance;
static std::unique_ptr<ll::i18n::I18N>    locale;

PlayerRegisterMod& PlayerRegisterMod::getInstance() { return *instance; }
std::string        PlayerRegisterMod::tr(const std::string& key) { return std::string(locale->get(key)); }

bool PlayerRegisterMod::load() {
    if(!Config::init()) return false;
    locale = std::make_unique<ll::i18n::MultiFileI18N>(ll::i18n::MultiFileI18N(NATIVE_MOD.getLangDir(), CONF.lang));
    return Database::init() && setupHooks();
}

bool PlayerRegisterMod::enable() { return Command::init(); }

bool PlayerRegisterMod::disable() { return true; }

} // namespace PlayerRegister

LL_REGISTER_MOD(PlayerRegister::PlayerRegisterMod, PlayerRegister::instance);
