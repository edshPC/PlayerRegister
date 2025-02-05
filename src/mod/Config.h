#pragma once

#include "Global.h"

#define CONF PlayerRegister::Config::getInstance()

namespace PlayerRegister {
class Config {
public:
    int version = 5;
    string lang = "en_US";
    int max_accounts = 3;
    bool reconnect = false;
    string reconnect_ip = "127.0.0.1";
    int reconnect_port = 19132;
    bool fake_uuid = true;
    bool fake_xuid = true;

    static bool init();
    static const Config& getInstance();

};
} // namespace PlayerRegister
