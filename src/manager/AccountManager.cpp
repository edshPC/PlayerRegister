#include "AccountManager.h"

#include "Database.h"
#include "ll/api/form/CustomForm.h"
#include "ll/api/form/FormBase.h"
#include "ll/api/form/SimpleForm.h"

#include <mc/entity/systems/common/PlayerDataSystem.h>
#include <util/SHA256.h>

#define CUSTOM_FORM_FUNC(name) void name(Player& pl, CustomFormResult const& res, FormCancelReason cancel)
#define FORM_GET(name, type)   auto name = std::get<type>(res->at(#name));

using namespace ll::form;

namespace PlayerRegister {

static void trim_string(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}

bool AccountManager::createAccount(Player& pl, const std::string& name, const std::string& password, bool create_new) {
    PlayerData data(PlayerManager::getId(&pl), name);
    Database::loadAsAccount(data);
    if (data.valid) return false; // account exists
    data.password = SHA256::digest_str(password);
    if (create_new) {
        data.fakeUUID  = mce::UUID::random();
        data.fakeDBkey = "player_server_" + mce::UUID::random().asString();
    } else {
        data.fakeUUID  = PlayerManager::getRealUUID(&pl);
        data.fakeDBkey = PlayerDataSystem::serverKey(LEVEL->getLevelStorage(), pl);
    }
    data.valid = true;
    Database::storeAsAccount(data);
    Database::storeAsPlayer(data);
    LOGGER.info("{} created an account {}", pl.getRealName(), data.name);
    if (create_new) PlayerManager::reconnect(&pl);
    else {
        PlayerManager::setPlayerData(&pl, data);
        pl.sendMessage(TR(register.success));
    }
    return true;
}
bool AccountManager::loginAccount(Player& pl, const std::string& name, const std::string& password) {
    PlayerData data(PlayerManager::getId(&pl), name);
    Database::loadAsAccount(data);
    // account not exists or incorrect pass
    if (!data.valid || data.password != SHA256::digest_str(password)) return false;
    Database::storeAsPlayer(data);
    LOGGER.info("{} logged in account {}", pl.getRealName(), data.name);
    PlayerManager::reconnect(&pl);
    return true;
}
bool AccountManager::changePassword(const std::string& name, const std::string& new_password) {
    PlayerData data("", name);
    Database::loadAsAccount(data);
    if (!data.valid) return false;
    data.password = SHA256::digest_str(new_password);
    Database::storeAsAccount(data);
    return true;
}

void AccountManager::loginOrRegisterForm(Player& pl, const string& repeat_reason, bool is_login) {
    CustomForm form{TR(form.log_or_reg.header)};
    if (!repeat_reason.empty()) form.appendLabel("§c" + repeat_reason + "§r\n\n");
    form.appendLabel(TR(form.log_or_reg.description));
    form.appendToggle("is_login", TR(from.is_login), is_login);
    form.appendInput("name", TR(form.reg.name), "", pl.getRealName());
    form.appendInput("password", TR(form.reg.password));
    form.sendTo(pl, [](Player& pl, CustomFormResult const& res, FormCancelReason cancel) {
        if (cancel.has_value()) return loginOrRegisterForm(pl);
        FORM_GET(is_login, uint64);
        FORM_GET(name, string);
        FORM_GET(password, string);
        trim_string(name);
        trim_string(password);
        if (is_login) {
            if (!loginAccount(pl, name, password)) loginOrRegisterForm(pl, TR(form.incorrect_pass), is_login);
            return;
        }
        if (name.length() < 4 || password.length() < 4)
            loginOrRegisterForm(pl, TR(form.name_or_pass_too_short), is_login);
        else if (!createAccount(pl, name, password)) loginOrRegisterForm(pl, TR(form.account_exists), is_login);
    });
}
void AccountManager::registerForm(Player& pl, const string& repeat_reason) {
    CustomForm form{TR(form.reg.header)};
    if (!repeat_reason.empty()) form.appendLabel("§c" + repeat_reason + "§r\n\n");
    form.appendInput("name", TR(form.reg.name), "", pl.getRealName());
    form.appendInput("password", TR(form.reg.password));
    form.sendTo(pl, [](Player& pl, CustomFormResult const& res, FormCancelReason cancel) {
        if (cancel.has_value()) return;
        FORM_GET(name, string);
        FORM_GET(password, string);
        trim_string(name);
        trim_string(password);
        PlayerData test(PlayerManager::getId(&pl));
        Database::loadAsPlayer(test);
        if (name.length() < 4 || password.length() < 4) registerForm(pl, TR(form.name_or_pass_too_short));
        else if (!createAccount(pl, name, password, test.valid)) registerForm(pl, TR(form.account_exists));
    });
}
void AccountManager::loginForm(Player& pl, const string& repeat_reason) {
    CustomForm form{TR(form.login.header)};
    if (!repeat_reason.empty()) form.appendLabel("§c" + repeat_reason + "§r\n\n");
    form.appendInput("name", TR(form.login.name), "", pl.getRealName());
    form.appendInput("password", TR(form.login.password));
    form.sendTo(pl, [](Player& pl, CustomFormResult const& res, FormCancelReason cancel) {
        if (cancel.has_value()) return;
        FORM_GET(name, string);
        FORM_GET(password, string);
        trim_string(name);
        trim_string(password);
        if (!loginAccount(pl, name, password)) loginForm(pl, TR(form.incorrect_pass));
    });
}
void AccountManager::infoForm(Player& pl) {
    auto&      data = PlayerManager::getPlayerData(&pl);
    SimpleForm form{TR(form.info.header), std::format(TR(form.info.description), data.name)};
    if (data.valid)
        form.appendButton(TR(form.info.change_password), [data](Player& pl) {
            CustomForm form{TR(form.change_password.header)};
            form.appendInput("password", TR(form.change_password.password));
            form.sendTo(pl, [data](Player& pl, CustomFormResult const& res, FormCancelReason cancel) {
                if (cancel.has_value()) return;
                FORM_GET(password, string);
                trim_string(password);
                if (password.length() < 4) pl.sendMessage("§c" + TR(form.name_or_pass_too_short));
                else if (changePassword(data.name, password)) pl.sendMessage(TR(form.change_password.success));
            });
        });
    form.sendTo(pl);
}

} // namespace PlayerRegister
