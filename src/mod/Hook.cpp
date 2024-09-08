#include "Global.h"

#include "ll/api/memory/Hook.h"

#include "PlayerManager.h"
#include "PlayerRegisterMod.h"

#include <mc/certificates/Certificate.h>
#include <mc/entity/systems/common/PlayerDataSystem.h>
#include <mc/network/ConnectionRequest.h>
#include <mc/network/ServerNetworkHandler.h>
#include <mc/network/packet/LoginPacket.h>
#include <mc/server/ServerPlayer.h>
#include <mc/entity/gamerefs_entity/EntityRefTraits.h>
#include <mc/entity/EntityContext.h>
#include <mc/world/level/storage/DBStorage.h>

namespace PlayerRegister {
// LL_TYPE_INSTANCE_HOOK(
//     spConstructorHook,
//     HookPriority::Normal,
//     ServerNetworkHandler,
//     &ServerNetworkHandler::createNewPlayer,
//     OwnerPtr<EntityContext>,
//     class NetworkIdentifier const& source,
//     class ConnectionRequest const& connectionRequest
// ) {
//     LOGGER.info("creating sp");
//     auto res = origin(source, connectionRequest);
//     auto sp = res.tryUnwrap<ServerPlayer>();
//     LOGGER.info("created sp {}", sp->getName());
//     PlayerManager::setFakeUUID(sp, "777b0b2e-92cb-39a8-bbc0-c36fc2da3bf3");
//     return res;
// }

LL_TYPE_INSTANCE_HOOK(
    spConstructorHook,
    HookPriority::Normal,
    ServerNetworkHandler,
    &ServerNetworkHandler::handle,
    void,
    class NetworkIdentifier const& source, class LoginPacket const& packet
) {
    origin(source, packet);
    auto clientId = packet.mConnectionRequest->getDeviceId();
    auto cert = packet.mConnectionRequest->getCertificate();
    auto uuid = packet.mConnectionRequest->mCertificate->getExtraData("identity", Json::Value("")).asString("");
    auto xuid = packet.mConnectionRequest->mCertificate->getExtraData("XUID", Json::Value("")).asString("");
    //PlayerManager::setFakeUUID(uuid, "777b0b2e-92cb-39a8-bbc0-c36fc2da3bf3");
    LOGGER.info("packet: {}", uuid);
}

LL_TYPE_INSTANCE_HOOK(fakeGetUuidHook, HookPriority::Normal, Player, &Player::getUuid, mce::UUID const&) {
    return mce::UUID::fromString(PlayerManager::getFakeUUID(this));
}

// LL_STATIC_HOOK(
//     fakePlayerKeyHook,
//     HookPriority::Normal,
//     &PlayerDataSystem::playerKey,
//     std::string,
//     std::string_view dat
// ) {
//     string res = "player_" + PlayerManager::getFakeUUID(string(dat));
//     LOGGER.info("playerKey {} -> {}", dat, res);
//     return res;
// }

LL_TYPE_INSTANCE_HOOK(
    DBStorageFakeLoadHook,
    HookPriority::Normal,
    DBStorage,
    "?loadData@DBStorage@@UEBA_NV?$basic_string_view@DU?$char_traits@D@std@@@std@@AEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@3@W4Category@DBHelpers@@@Z",
    bool,
    std::string_view key, std::string& buffer, ::DBHelpers::Category category
) {
    if (category != DBHelpers::Category::Player) return origin(key, buffer, category);
    LOGGER.info("Loading db string {}", key);
    return origin(PlayerManager::getFakeDBkey(string(key)), buffer, category);
}
LL_TYPE_INSTANCE_HOOK(
    DBStorageFakeSaveHook,
    HookPriority::Normal,
    DBStorage,
    "?saveData@DBStorage@@UEAA?AV?$shared_ptr@V?$IAsyncResult@X@Threading@Bedrock@@@std@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@3@$$QEAV43@W4Category@DBHelpers@@@Z",
    std::shared_ptr<Bedrock::Threading::IAsyncResult<void>>,
    std::string const& key, std::string& data, ::DBHelpers::Category category
) {
    if (category != DBHelpers::Category::Player) return origin(key, data, category);
    LOGGER.info("Saveing db string {}", key);
    return origin(PlayerManager::getFakeDBkey(key), data, category);
}

bool setupHooks() {
    spConstructorHook::hook();
    fakeGetUuidHook::hook();
    //fakePlayerKeyHook::hook();
    DBStorageFakeLoadHook::hook();
    DBStorageFakeSaveHook::hook();
    return true;
}

} // namespace PlayerRegister
