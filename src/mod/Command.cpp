#include "Command.h"

#include "manager/AccountManager.h"

#include <ll/api/command/CommandHandle.h>
#include <ll/api/command/CommandRegistrar.h>
#include <ll/api/command/Command.h>
#include <mc/server/commands/CommandOutput.h>

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
    case Command::info:
        AccountManager::infoForm(*pl);
        break;
    }
    out.mSuccessCount++;
}

EXECUTE_CMD(ResetPass) {
    ori;
    string name     = param.account_name.mText;
    string password = mce::UUID::random().asString().substr(0, 8);
    if (!AccountManager::changePassword(name, password)) return out.error(TR(command.reset_password.not_found));
    LOGGER.warn(std::vformat(TR(command.reset_password.success), std::make_format_args(name, password)));
    out.mSuccessCount++;
}

bool Command::init() {
    if (!ll::service::getCommandRegistry().has_value()) return false;
    auto& registrar = CommandRegistrar::getInstance();

    registrar.tryRegisterEnum<AccountAction>();
    auto& cmd = registrar.getOrCreateCommand("account", TR(command.account.description));
    cmd.overload<AccountParam>().required("action").execute(&executeAccount);

    auto& cmd1 = registrar.getOrCreateCommand(
        "resetpassword",
        TR(command.reset_password.description),
        CommandPermissionLevel::Host
    );
    cmd1.overload<ResetPassParam>().required("account_name").execute(&executeResetPass);

    return true;
}
} // namespace PlayerRegister
