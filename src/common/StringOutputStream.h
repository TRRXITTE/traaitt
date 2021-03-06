// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2018-2020, The TurtleCoin Developers // Copyright (c) 2020, TRRXITTE inc.
//
// Please see the included LICENSE file for more information.

#pragma once

#include "IOutputStream.h"

#include <string>

namespace Common
{
    class StringOutputStream : public IOutputStream
    {
      public:
        StringOutputStream(std::string &out);

        uint64_t writeSome(const void *data, uint64_t size) override;

      private:
        std::string &out;
    };

} // namespace Common
