#include "Database.h"

#include <ll/api/data/KeyValueDB.h>

#include "Global.h"
#include "PlayerRegisterMod.h"

namespace PlayerRegister {
ll::data::KeyValueDB* db;

bool Database::init() {
    db = new ll::data::KeyValueDB(NATIVE_MOD.getDataDir());
    return true;
}
void Database::storePlayer(std::string id, PlayerData* data) {
    json j;
    j["fakeUUID"] = data->fakeUUID;
    db->set(id, j.dump());
}
PlayerData* Database::loadPlayer(std::string id) {
    auto res = db->get(id);
    if(!res.has_value()) return nullptr;
    json j = json::parse(res.value());
    PlayerData* data = new PlayerData();
    data->fakeUUID = j["fakeUUID"].get<std::string>();
    return data;
}

} // namespace PlayerRegister
