#pragma once

#include "Global.h"
#include "PlayerManager.h"

namespace PlayerRegister {
class Database {
public:
    static bool init();

    static void storePlayer(const std::string& id, PlayerData* data);
    static PlayerData* loadPlayer(const std::string& id);

    static void storeAccount(AccountInfo* info);
    static AccountInfo* loadAccount(const std::string& name);
};
}
