#pragma once

#include "Global.h"
#include "PlayerManager.h"

namespace PlayerRegister {
struct AccountInfo {
    string name;
    string password_hash;
    PlayerData* data;
};
class AccountManager {
public:
    static void createNewAccount(Player& pl);

};
} // namespace PlayerRegister
