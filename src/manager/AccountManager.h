#pragma once

#include "Global.h"
#include "PlayerManager.h"

namespace PlayerRegister {

class AccountManager {
public:
    static bool createAccount(Player& pl, const std::string& name, const std::string& password, bool create_new = false);
    static bool loginAccount(Player& pl, const std::string& name, const std::string& password);
    static bool changePassword(const std::string& name, const std::string& new_password);

    static void loginOrRegisterForm(Player& pl, const string& repeat_reason = {}, bool is_login = false);
    static void registerForm(Player& pl, const string& repeat_reason = {});
    static void loginForm(Player& pl, const string& repeat_reason = {});
    static void infoForm(Player& pl);

};
} // namespace PlayerRegister
