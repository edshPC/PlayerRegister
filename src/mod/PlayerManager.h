#pragma once

#include "Global.h"

using std::string;

namespace PlayerRegister {
struct PlayerData {
    string fakeUUID;
};

class PlayerManager {
public:

    static string getRealUUID(Player* pl);
    static string getFakeUUID(Player* pl);

    static std::string getFakeDBkey(const std::string& key);

    static void setFakeUUID(string real, string fake);

    static string getId(Player* pl);

};
}
