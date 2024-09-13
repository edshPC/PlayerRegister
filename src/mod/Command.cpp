#include "Command.h"

#include "manager/AccountManager.h"

#include <ll/api/command/CommandHandle.h>
#include <ll/api/command/CommandRegistrar.h>
#include <ll/api/command/runtime/ParamKind.h>
#include <mc/world/level/Command.h>

#define EXECUTE_CMD(name)                                                                                              \
    void execute##name(const CommandOrigin& ori, CommandOutput& out, const Command::##name##Param& param)

using namespace ll::command;

namespace PlayerRegister {

EXECUTE_CMD(Account) {
    if (ori.getOriginType() != CommandOriginType::Player) return out.error(TR(command.not_a_player));
    auto pl = static_cast<Player*>(ori.getEntity());
    switch (param.action) {
    case Command::create:
        AccountManager::registerForm(*pl);
        break;
    case Command::login:
        AccountManager::loginForm(*pl);
        break;
    }
    out.success();
}

bool Command::init() {
    if (!ll::service::getCommandRegistry().has_value()) return false;
    auto& registrar = CommandRegistrar::getInstance();

    registrar.tryRegisterEnum<AccountAction>();

    auto& cmd = registrar.getOrCreateCommand("account", TR(command.description));
    cmd.overload<AccountParam>().required("action").execute(&executeAccount);

    return true;
}
} // namespace PlayerRegister
