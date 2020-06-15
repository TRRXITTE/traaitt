// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2018-2020, The TurtleCoin Developers // Copyright (c) 2020, TRRXITTE inc.
//
// Please see the included LICENSE file for more information.

#pragma once

#include <cstdint>

namespace CryptoNote
{
    class ICoreInformation
    {
      public:
        virtual ~ICoreInformation() {}

        virtual size_t getPoolTransactionCount() const = 0;

        virtual size_t getBlockchainTransactionCount() const = 0;

        virtual size_t getAlternativeBlockCount() const = 0;

        virtual std::vector<Transaction> getPoolTransactions() const = 0;
    };

} // namespace CryptoNote
