#include "PlayerManager.h"

#include "Database.h"

#include <mc/entity/systems/common/PlayerDataSystem.h>
#include <mc/network/ConnectionRequest.h>

namespace PlayerRegister {

unordered_map<Player*, PlayerData*>     playersData;
unordered_map<std::string, std::string> fakeDBkeys; // player_server_{uuid}

const mce::UUID& PlayerManager::getRealUUID(Player* pl) { return pl->getUserEntityIdentifier().mClientUUID; }
const mce::UUID& PlayerManager::getFakeUUID(Player* pl) {
    if (playersData.contains(pl)) return playersData.at(pl)->fakeUUID;
    return getRealUUID(pl);
}
std::string PlayerManager::getFakeDBkey(const std::string& key) {
    if (fakeDBkeys.contains(key)) {
        LOGGER.info("{} -> {}", key, fakeDBkeys.at(key));
        return fakeDBkeys.at(key);
    }
    return key;
}
void PlayerManager::setFakeDBkey(Player* pl, string fake) {
    auto real        = PlayerDataSystem::serverKey(LEVEL->getLevelStorage(), *pl);
    fakeDBkeys[real] = "player_server_" + fake;
}
void   PlayerManager::loadPlayer(Player* pl) {
    if (auto data = Database::loadPlayer(getId(pl))) {
        playersData[pl] = data;
    }


}
string PlayerManager::getId(Player* pl) {
    string xuid = pl->getXuid();
    if (xuid.empty()) return pl->getConnectionRequest()->getDeviceId();
    return xuid;
}
} // namespace PlayerRegister
