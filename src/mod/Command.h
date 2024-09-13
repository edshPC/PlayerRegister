#pragma once

#include "Global.h"
#include <ll/api/command/runtime/ParamKind.h>

namespace PlayerRegister {
class Command {
public:
    enum AccountAction {
        create,
        login,
        info,
    };
    struct AccountParam {
        AccountAction action;
    };
    struct ResetPassParam {
        CommandRawText account_name;
    };

    static bool init();
};
} // namespace PlayerRegister
