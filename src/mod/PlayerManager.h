#pragma once

#include "Global.h"

using std::string;

namespace PlayerRegister {
struct PlayerData {
    mce::UUID fakeUUID;
};

class PlayerManager {
public:

    static const mce::UUID& getRealUUID(Player* pl);
    static const mce::UUID& getFakeUUID(Player* pl);

    static std::string getFakeDBkey(const std::string& key);
    static void setFakeDBkey(Player* pl, string fake);

    static void loadPlayer(Player* pl);

    static string getId(Player* pl);

};
}
