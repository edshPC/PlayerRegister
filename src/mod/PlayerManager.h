#pragma once

#include "AccountManager.h"
#include "Global.h"

using std::string;

namespace PlayerRegister {
struct PlayerData {
    mce::UUID fakeUUID;
    std::string fakeDBkey;
    AccountInfo* account;
};

class PlayerManager {
public:

    static const mce::UUID& getRealUUID(Player* pl);
    static const mce::UUID& getFakeUUID(Player* pl);

    static std::string getFakeDBkey(const std::string& key);
    static void setFakeDBkey(Player* pl);
    static void setPlayerData(Player* pl, PlayerData* data);

    static void loadPlayer(Player* pl);
    static void unloadPlayer(Player* pl);

    static string getId(Player* pl);

};
}
