#pragma once

#include "AccountManager.h"
#include "Global.h"

using std::string;

namespace PlayerRegister {
struct PlayerData {
    string id;
    string name;
    string password;
    int accounts = 0;

    mce::UUID fakeUUID;
    std::string fakeDBkey;

    bool valid = false;
};

class PlayerManager {
public:

    static const mce::UUID& getRealUUID(Player* pl);
    static const mce::UUID& getFakeUUID(Player* pl);

    static std::string getFakeDBkey(const std::string& real);
    static void setFakeDBkey(Player* pl);
    static void setPlayerData(Player* pl, PlayerData& data);

    static void loadPlayer(Player* pl);
    static void unloadPlayer(Player* pl);

    static PlayerData& getPlayerData(Player* pl);
    static string getId(Player* pl);
    static void reconnect(Player* pl);
};
}
