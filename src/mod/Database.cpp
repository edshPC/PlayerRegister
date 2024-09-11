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

static void serializeData(json& j, PlayerData* data) {
    j["fakeUUID"]  = data->fakeUUID.asString();
    j["fakeDBkey"] = data->fakeDBkey;
}
static PlayerData* deserializeData(const json& j) {
    PlayerData* data = new PlayerData();
    data->fakeUUID   = j["fakeUUID"].get<std::string>();
    data->fakeDBkey  = j["fakeDBkey"].get<std::string>();
    return data;
}

void Database::storePlayer(const std::string& id, PlayerData* data) {
    json j;
    serializeData(j, data);
    db->set("player_" + id, j.dump());
}
PlayerData* Database::loadPlayer(const std::string& id) {
    auto res = db->get("player_" + id);
    if (!res.has_value()) return nullptr;
    json j = json::parse(res.value());
    return deserializeData(j);
}

void Database::storeAccount(AccountInfo* info) {
    json j;
    serializeData(j, info->data);
    j["password"] = info->password_hash;
    db->set("account_" + info->name, j.dump());
}
AccountInfo* Database::loadAccount(const std::string& name) {
    auto res = db->get("account_" + name);
    if (!res.has_value()) return nullptr;
    json j = json::parse(res.value());
    AccountInfo* info = new AccountInfo();
    info->name = name;
    info->password_hash = j["password"].get<std::string>();
    info->data = deserializeData(j);
    return info;
}

} // namespace PlayerRegister
