#pragma once

#include "Global.h"

#define CONF PlayerRegister::Config::getInstance()

namespace PlayerRegister {
class Config {
public:
    int version = 1;

    bool reconnect = false;
    string reconnect_ip = "127.0.0.1";
    int reconnect_port = 19132;

    static bool init();
    static const Config& getInstance();

};
} // namespace PlayerRegister
