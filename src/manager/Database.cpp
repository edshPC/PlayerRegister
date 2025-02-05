#include "Database.h"

#include <ll/api/data/KeyValueDB.h>

#include "Global.h"

namespace PlayerRegister {
std::unique_ptr<ll::data::KeyValueDB> db;

bool Database::init() {
    db = std::make_unique<ll::data::KeyValueDB>(NATIVE_MOD.getDataDir());
    return true;
}

static void serializeData(json& j, const PlayerData& data) {
    j["name"]      = data.name;
    j["password"]  = data.password;
    j["accounts"]  = data.accounts;
    j["fakeUUID"]  = data.fakeUUID.asString();
    j["fakeXUID"]  = data.fakeXUID;
    j["fakeDBkey"] = data.fakeDBkey;
}
static void deserializeData(const json& j, PlayerData& data) {
    data.name      = j["name"].get<std::string>();
    data.password  = j["password"].get<std::string>();
    data.accounts  = j["accounts"].get<int>();
    data.fakeUUID  = j["fakeUUID"].get<std::string>();
    data.fakeXUID  = j["fakeXUID"].get<std::string>();
    data.fakeDBkey = j["fakeDBkey"].get<std::string>();
}

void Database::storeAsPlayer(const PlayerData& data) {
    json j;
    serializeData(j, data);
    db->set("player_" + data.id, j.dump());
}
void Database::loadAsPlayer(PlayerData& data) {
    auto res = db->get("player_" + data.id);
    if (!res.has_value()) return;
    json j = json::parse(res.value());
    deserializeData(j, data);
    data.valid = true;
}

void Database::storeAsAccount(const PlayerData& data) {
    json j;
    serializeData(j, data);
    db->set("account_" + data.name, j.dump());
}
void Database::loadAsAccount(PlayerData& data) {
    auto res = db->get("account_" + data.name);
    if (!res.has_value()) return;
    json j = json::parse(res.value());
    deserializeData(j, data);
    data.valid = true;
}

} // namespace PlayerRegister
