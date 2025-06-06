#include "PlayerManager.h"

#include "Database.h"
#include "mod/Config.h"

#include <mc/world/level/storage/PlayerDataSystem.h>
#include <mc/network/ConnectionRequest.h>
#include <mc/network/packet/TransferPacket.h>

namespace PlayerRegister {

unordered_map<Player*, PlayerData>      playersData;
unordered_map<std::string, std::string> fakeDBkeys; // player_server_{uuid}
unordered_map<mce::UUID, Player*>       playerUUIDs;

const PlayerData emptyData{"null", "[Not registered]"};

const mce::UUID& PlayerManager::getRealUUID(Player* pl) { return pl->getUserEntityIdentifier().mClientUUID; }
const mce::UUID& PlayerManager::getFakeUUID(Player* pl) {
    if (playersData.contains(pl)) return playersData.at(pl).fakeUUID;
    return getRealUUID(pl);
}
std::string PlayerManager::getFakeDBkey(const std::string& real) {
    if (fakeDBkeys.contains(real)) return fakeDBkeys.at(real);
    return real;
}
void PlayerManager::setFakeDBkey(Player* pl) {
    if (!playersData.contains(pl)) return;
    string real      = PlayerDataSystem::serverKey(LEVEL->getLevelStorage(), *pl);
    fakeDBkeys[real] = playersData.at(pl).fakeDBkey;
}
void PlayerManager::setPlayerData(Player* pl, PlayerData& data) {
    playersData[pl] = data;
    playerUUIDs[pl->getUuid()] = pl;
    if(!data.fakeDBkey.empty()) setFakeDBkey(pl);
}
void PlayerManager::loadPlayer(Player* pl) {
    PlayerData data{getId(pl)};
    Database::loadAsPlayer(data);
    if (data.valid) {
        for (auto& entry : playersData)
            if (entry.second.name == data.name) {
                entry.first->disconnect(TR(player.join_from_other));
                break;
            }
        return setPlayerData(pl, data);
    }
    // No xbox-authed players should register accounts
    if (pl->getXuid().empty()) AccountManager::loginOrRegisterForm(*pl);
}
void PlayerManager::unloadPlayer(Player* pl) {
    playersData.erase(pl);
    playerUUIDs.erase(pl->getUuid());
    fakeDBkeys.erase(PlayerDataSystem::serverKey(LEVEL->getLevelStorage(), *pl));
}
const PlayerData& PlayerManager::getPlayerData(Player* pl) {
    if (playersData.contains(pl)) return playersData.at(pl);
    return emptyData;
}
Player* PlayerManager::getPlayerByUUID(const mce::UUID& uuid) {
    if (playerUUIDs.contains(uuid)) return playerUUIDs.at(uuid);
    return nullptr;
}
const unordered_map<Player*, PlayerData>& PlayerManager::getAllData() {
    return playersData;
}
string PlayerManager::getId(Player* pl) {
    return pl->getConnectionRequest()->getDeviceId();
}
void PlayerManager::reconnect(Player* pl) {
    if (CONF.reconnect) {
        TransferPacket pkt;
        pkt.mServerAddress = CONF.reconnect_ip;
        pkt.mServerPort = CONF.reconnect_port;
        pkt.mReloadWorld = false;
        pl->sendNetworkPacket(pkt);
    } else pl->disconnect(TR(player.reconnect));
}
} // namespace PlayerRegister
