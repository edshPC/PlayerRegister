#include "Global.h"

#include "ll/api/memory/Hook.h"

#include "Config.h"
#include "PlayerRegisterMod.h"
#include "manager/PlayerManager.h"

#include <mc/deps/ecs/gamerefs_entity/EntityContext.h>
#include <mc/deps/ecs/gamerefs_entity/GameRefsEntity.h>
#include <mc/network/ConnectionRequest.h>
#include <mc/network/ServerNetworkHandler.h>
#include <mc/server/ServerPlayer.h>
#include <mc/world/level/storage/DBStorage.h>

namespace PlayerRegister {
LL_TYPE_INSTANCE_HOOK(
    CreateNewPlayerHook,
    HookPriority::High,
    ServerNetworkHandler,
    &createNewPlayer,
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
    HookPriority::High,
    ServerNetworkHandler,
    &_onPlayerLeft,
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
    &$loadData,
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
    &$saveData,
    std::shared_ptr<Bedrock::Threading::IAsyncResult<void>>,
    std::string const&    key,
    std::string&&         data,
    ::DBHelpers::Category category
) {
    if (category != DBHelpers::Category::Player) return origin(key, std::move(data), category);
    return origin(PlayerManager::getFakeDBkey(key), std::move(data), category);
}

// LeviLamina method hook to support other mods
LL_TYPE_INSTANCE_HOOK(FakeGetUuidHook, HookPriority::High, Player, &getUuid, mce::UUID const&) {
    return PlayerManager::getFakeUUID(this);
}

LL_TYPE_INSTANCE_HOOK(FakeGetXuidHook, HookPriority::High, Player, &$getXuid, string) {
    auto& data = PlayerManager::getPlayerData(this);
    if (data.fakeXUID.empty()) return origin();
    return data.fakeXUID;
}

LL_TYPE_INSTANCE_HOOK(
    FakeGetPlayerHook,
    HookPriority::Highest,
    Level,
    &$getPlayer,
    Player*,
    mce::UUID const& uuid
) {
    if (auto pl = PlayerManager::getPlayerByUUID(uuid); pl) {
        return pl;
    }
    return origin(uuid);
}

bool setupHooks() {
    CreateNewPlayerHook::hook();
    OnPlayerLeftHook::hook();
    DBStorageFakeLoadHook::hook();
    DBStorageFakeSaveHook::hook();
    if (CONF.fake_uuid) {
        FakeGetUuidHook::hook();
        FakeGetPlayerHook::hook();
    }
    if (CONF.fake_xuid) {
        FakeGetXuidHook::hook();
    }
    return true;
}

} // namespace PlayerRegister
