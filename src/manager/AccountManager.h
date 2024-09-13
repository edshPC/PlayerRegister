#pragma once

#include "Global.h"
#include "PlayerManager.h"

namespace PlayerRegister {

class AccountManager {
public:
    static bool createAccount(Player& pl, const std::string& name, const std::string& password, bool create_new = false);
    static bool loginAccount(Player& pl, const std::string& name, const std::string& password);

    static void loginOrRegisterForm(Player& pl, const string& repeat_reason = {}, bool is_login = false);
    static void registerForm(Player& pl, const string& repeat_reason = {});
    static void loginForm(Player& pl, const string& repeat_reason = {});

};
} // namespace PlayerRegister
