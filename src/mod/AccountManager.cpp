#include "AccountManager.h"

#include "Database.h"
#include "ll/api/form/CustomForm.h"
#include "ll/api/form/FormBase.h"

#include <util/SHA256.h>

#define FORM_GET(name, type) auto name = std::get<type>(res->at(#name));

using namespace ll::form;

namespace PlayerRegister {

void AccountManager::createNewAccount(Player& pl) {
    CustomForm form{"§bРегистрация"};
    form.appendInput("name", "", "name", pl.getRealName());
    form.appendInput("password", "", "password");
    form.sendTo(pl, [](Player& pl, CustomFormResult const& res, FormCancelReason cancel){
        if (cancel.has_value()) return createNewAccount(pl);
        FORM_GET(name, string);
        if(Database::loadAccount(name)) return createNewAccount(pl);
        FORM_GET(password, string);
        string pass_hash = SHA256::digest_str(password);

    });
}

} // namespace PlayerRegister
