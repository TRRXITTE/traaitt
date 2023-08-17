// Copyright (c) 2018-2020, The TurtleCoin Developers // Copyright (c) 2020, TRRXITTE inc.
//
// Please see the included LICENSE file for more information.

#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <walletbackend/WalletBackend.h>

namespace ZedUtilities
{
    void confirmPassword(const std::shared_ptr<WalletBackend> walletBackend, const std::string msg);

    uint64_t getScanHeight();

} // namespace ZedUtilities
