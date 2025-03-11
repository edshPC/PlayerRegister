#pragma once

#include "Global.h"
#include "PlayerManager.h"

namespace PlayerRegister {
class Database {
public:
    static bool init();

    static void storeAsPlayer(const PlayerData& data);
    static void loadAsPlayer(PlayerData& data);
    static bool removePlayer(const std::string& id);

    static void storeAsAccount(const PlayerData& data);
    static void loadAsAccount(PlayerData& data);
};
}
