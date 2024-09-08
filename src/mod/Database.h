#pragma once

#include "Global.h"
#include "PlayerManager.h"

namespace PlayerRegister {
class Database {
public:
    static bool init();
    static void storePlayer(std::string id, PlayerData* data);
    static PlayerData* loadPlayer(std::string id);
};
}
