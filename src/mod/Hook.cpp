#include "Global.h"

#include "ll/api/memory/Hook.h"

#include "Config.h"
#include "PlayerRegisterMod.h"
#include "manager/PlayerManager.h"

#include <mc/certificates/Certificate.h>
#include <mc/entity/EntityContext.h>
#include <mc/entity/gamerefs_entity/EntityRefTraits.h>
#include <mc/network/ConnectionRequest.h>
#include <mc/network/ServerNetworkHandler.h>
#include <mc/network/packet/LoginPacket.h>
#include <mc/server/ServerPlayer.h>
#include <mc/world/level/storage/DBStorage.h>

namespace PlayerRegister {
LL_TYPE_INSTANCE_HOOK(
    CreateNewPlayerHook,
    HookPriority::High,
    ServerNetworkHandler,
    &ServerNetworkHandler::createNewPlayer,
    OwnerPtr<EntityContext>,
    class NetworkIdentifier const& source,
    class ConnectionRequest const& connectionRequest
) {
    auto res = origin(source, connectionRequest);
    auto sp  = res.tryUnwrap<ServerPlayer>();
    if (sp.has_value()) PlayerManager::loadPlayer(sp);
    return res;
}
LL_TYPE_INSTANCE_HOOK(
    OnPlayerLeftHook,
    HookPriority::Low,
    ServerNetworkHandler,
    &ServerNetworkHandler::_onPlayerLeft,
    void,
    ServerPlayer* player,
    bool          skipMessage
) {
    origin(player, skipMessage);
    PlayerManager::unloadPlayer(player);
}

LL_TYPE_INSTANCE_HOOK(
    DBStorageFakeLoadHook,
    HookPriority::Normal,
    DBStorage,
    "?loadData@DBStorage@@UEBA_NV?$basic_string_view@DU?$char_traits@D@std@@@std@@AEAV?$basic_string@DU?$char_traits@D@"
    "std@@V?$allocator@D@2@@3@W4Category@DBHelpers@@@Z",
    bool,
    std::string_view      key,
    std::string&          buffer,
    ::DBHelpers::Category category
) {
    if (category != DBHelpers::Category::Player) return origin(key, buffer, category);
    return origin(PlayerManager::getFakeDBkey(string(key)), buffer, category);
}
LL_TYPE_INSTANCE_HOOK(
    DBStorageFakeSaveHook,
    HookPriority::Normal,
    DBStorage,
    "?saveData@DBStorage@@UEAA?AV?$shared_ptr@V?$IAsyncResult@X@Threading@Bedrock@@@std@@AEBV?$basic_string@DU?$char_"
    "traits@D@std@@V?$allocator@D@2@@3@$$QEAV43@W4Category@DBHelpers@@@Z",
    std::shared_ptr<Bedrock::Threading::IAsyncResult<void>>,
    std::string const&    key,
    std::string&          data,
    ::DBHelpers::Category category
) {
    if (category != DBHelpers::Category::Player) return origin(key, data, category);
    return origin(PlayerManager::getFakeDBkey(key), data, category);
}

// LeviLamina method hook to support other mods
LL_TYPE_INSTANCE_HOOK(FakeGetUuidHook, HookPriority::Normal, Player, &Player::getUuid, mce::UUID const&) {
    return PlayerManager::getFakeUUID(this);
}

LL_TYPE_INSTANCE_HOOK(FakeGetPlayerHook, HookPriority::Normal, Level, &Level::getPlayer, Player*, mce::UUID const& uuid) {
    if (Player* ori = origin(uuid)) return ori;
    for (auto& en : PlayerManager::getAllData()) {
        if (en.second.fakeUUID == uuid) return en.first;
    }
    return nullptr;
}

bool setupHooks() {
    CreateNewPlayerHook::hook();
    OnPlayerLeftHook::hook();
    DBStorageFakeLoadHook::hook();
    DBStorageFakeSaveHook::hook();
    if (CONF.fake_ll_uuid) {
        FakeGetUuidHook::hook();
        FakeGetPlayerHook::hook();
    }
    return true;
}

} // namespace PlayerRegister
