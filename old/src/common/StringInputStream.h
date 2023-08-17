// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2018-2020, The TurtleCoin Developers // Copyright (c) 2020, TRRXITTE inc.
//
// Please see the included LICENSE file for more information.

#pragma once

#include "IInputStream.h"

#include <string>

namespace Common
{
    class StringInputStream : public IInputStream
    {
      public:
        StringInputStream(const std::string &in);

        uint64_t readSome(void *data, uint64_t size) override;

      private:
        const std::string &in;

        uint64_t offset;
    };

} // namespace Common
