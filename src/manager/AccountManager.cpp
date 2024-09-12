#include "AccountManager.h"

#include "Database.h"
#include "ll/api/form/CustomForm.h"
#include "ll/api/form/FormBase.h"

#include <mc/entity/systems/common/PlayerDataSystem.h>
#include <util/SHA256.h>

#define CUSTOM_FORM_FUNC(name) void name(Player& pl, CustomFormResult const& res, FormCancelReason cancel)
#define FORM_GET(name, type)   auto name = std::get<type>(res->at(#name));

using namespace ll::form;

namespace PlayerRegister {

bool AccountManager::createAccount(Player& pl, const std::string& name, const std::string& password, bool create_new) {
    PlayerData data(PlayerManager::getId(&pl), name);
    Database::loadAsAccount(data);
    if (data.valid) return false; // account exists
    data.password = SHA256::digest_str(password);
    if (create_new) {
        data.fakeUUID  = mce::UUID::random();
        data.fakeDBkey = "player_server_" + mce::UUID::random().asString();
    } else {
        data.fakeUUID  = pl.getUuid();
        data.fakeDBkey = PlayerManager::getFakeDBkey(PlayerDataSystem::serverKey(LEVEL->getLevelStorage(), pl));
    }
    data.valid = true;
    Database::storeAsAccount(data);
    Database::storeAsPlayer(data);
    if (create_new) PlayerManager::reconnect(&pl);
    else {
        PlayerManager::setPlayerData(&pl, data);
        pl.sendMessage(TR(form.register.success));
    }
    return true;
}
bool AccountManager::loginAccount(Player& pl, const std::string& name, const std::string& password) {
    PlayerData data(PlayerManager::getId(&pl), name);
    Database::loadAsAccount(data);
    // account not exists or incorrect pass
    if (!data.valid || data.password != SHA256::digest_str(password)) return false;
    Database::storeAsPlayer(data);
    PlayerManager::reconnect(&pl);
    return true;
}

void AccountManager::loginOrRegisterForm(Player& pl, const string& repeat_reason, bool is_login) {
    CustomForm form{TR(form.log_or_reg.name)};
    string     description = TR(form.log_or_reg.description);
    if (!repeat_reason.empty()) description.insert(0, repeat_reason + "\n\n");
    form.appendToggle("is_login", description, is_login);
    form.appendInput("name", TR(form.name), "name", pl.getRealName());
    form.appendInput("password", TR(form.password), "password");
    form.sendTo(pl, [](Player& pl, CustomFormResult const& res, FormCancelReason cancel) {
        if (cancel.has_value()) return loginOrRegisterForm(pl);
        FORM_GET(is_login, uint64);
        FORM_GET(name, string);
        FORM_GET(password, string);
        if (is_login) {
            if (!loginAccount(pl, name, password)) loginOrRegisterForm(pl, TR(form.incorrect_pass), is_login);
            return;
        }
        if (name.length() < 4 || password.length() < 4)
            loginOrRegisterForm(pl, TR(form.name_or_pass_too_short), is_login);
        else if (!createAccount(pl, name, password)) loginOrRegisterForm(pl, TR(form.account_exists), is_login);
    });
}

} // namespace PlayerRegister
