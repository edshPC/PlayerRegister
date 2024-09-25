#pragma once

#include "Global.h"

#define CONF PlayerRegister::Config::getInstance()

namespace PlayerRegister {
class Config {
public:
    int version = 3;
    int max_accounts = 3;
    bool reconnect = false;
    string reconnect_ip = "127.0.0.1";
    int reconnect_port = 19132;
    bool fake_ll_uuid = true;

    static bool init();
    static const Config& getInstance();

};
} // namespace PlayerRegister
