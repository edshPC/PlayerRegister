#include "PlayerManager.h"

#include "Database.h"
#include "mod/Config.h"

#include <mc/entity/systems/common/PlayerDataSystem.h>
#include <mc/network/ConnectionRequest.h>
#include <mc/network/packet/TransferPacket.h>

namespace PlayerRegister {

unordered_map<Player*, PlayerData>      playersData;
unordered_map<std::string, std::string> fakeDBkeys; // player_server_{uuid}

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
    setFakeDBkey(pl);
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
    if (!pl->getXuid().empty()) return; // No need to auto-create account for xbox-authed players
    AccountManager::loginOrRegisterForm(*pl);
}
void PlayerManager::unloadPlayer(Player* pl) {
    playersData.erase(pl);
    fakeDBkeys.erase(PlayerDataSystem::serverKey(LEVEL->getLevelStorage(), *pl));
}
string PlayerManager::getId(Player* pl) {
    if (string xuid = pl->getXuid(); !xuid.empty()) return xuid;
    return pl->getConnectionRequest()->getDeviceId();
}
void PlayerManager::reconnect(Player* pl) {
    LEVEL->save();
    if (CONF.reconnect) {
        TransferPacket pkt{CONF.reconnect_ip, CONF.reconnect_port};
        pl->sendNetworkPacket(pkt);
    } else pl->disconnect(TR(player.reconnect));
}
} // namespace PlayerRegister
