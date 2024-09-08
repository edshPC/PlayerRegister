#include "PlayerManager.h"

#include <mc/network/ConnectionRequest.h>

namespace PlayerRegister {

unordered_map<Player*, PlayerData>      playersData;
unordered_map<std::string, std::string> fakeDBkeys; // player_{uuid}

string      PlayerManager::getRealUUID(Player* pl) { return pl->getUserEntityIdentifier().mClientUUID.asString(); }
std::string PlayerManager::getFakeUUID(Player* pl) {
    try {
        return playersData.at(pl).fakeUUID;
    } catch (...) {}
    return "";
}
std::string PlayerManager::getFakeDBkey(const std::string& key) {
    if (fakeDBkeys.contains(key)) {
        LOGGER.info("{} -> {}", key, fakeDBkeys.at(key));
        return fakeDBkeys.at(key);
    }
    return key;
}
void   PlayerManager::setFakeUUID(string real, string fake) { fakeDBkeys[real] = fake; }
string PlayerManager::getId(Player* pl) {
    string xuid = pl->getXuid();
    if (xuid.empty()) return pl->getConnectionRequest()->getDeviceId();
    return xuid;
}
} // namespace PlayerRegister
