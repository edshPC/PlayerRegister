#include "Config.h"

#include "ll/api/Config.h"

using namespace ll::config;

namespace PlayerRegister {

Config config;

bool Config::init() {
    try {
        if (!loadConfig(config, NATIVE_MOD.getConfigDir() / "config.json"))
            saveConfig(config, NATIVE_MOD.getConfigDir() / "config.json");
    } catch (...) {
        return false;
    }
    return true;
}
const Config& Config::getInstance() { return config; }

} // namespace PlayerRegister
