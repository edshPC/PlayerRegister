#pragma once

#include "Global.h"

namespace PlayerRegister {
class Command {
public:
    enum AccountAction {
        create,
        login,
    };
    struct AccountParam {
        AccountAction action;
    };

    static bool init();
};
} // namespace PlayerRegister
