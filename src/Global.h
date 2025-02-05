#pragma once

#include <string>

#include "ll/api/io/Logger.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/level/Level.h"
#include "ll/api/service/Bedrock.h"
#include "nlohmann/json.hpp"

#include "mod/PlayerRegisterMod.h"

#define NATIVE_MOD PlayerRegister::PlayerRegisterMod::getInstance().getSelf()
#define LOGGER NATIVE_MOD.getLogger()
#define LEVEL ll::service::getLevel()
#define TR(key) PlayerRegister::PlayerRegisterMod::tr(#key)

using namespace std;
using std::string;
using std::to_string;
using nlohmann::json;
