// Copyright (c) 2018-2020, The TurtleCoin Developers 
// Copyright (c) 2020, TRRXITTE inc.
//
// Please see the included LICENSE file for more information.

#pragma once

#include <walletbackend/WalletBackend.h>
#include <zedwallet++/ParseArguments.h>

bool handleCommand(
    const std::string command,
    const std::shared_ptr<WalletBackend> walletBackend,
    const std::shared_ptr<std::mutex> mutex);

std::shared_ptr<WalletBackend> handleLaunchCommand(const std::string launchCommand, const ZedConfig &config);
