// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2018-2020, The TurtleCoin Developers // Copyright (c) 2020, TRRXITTE inc.
//
// Please see the included LICENSE file for more information.

#pragma once

#include <exception>

namespace System
{
    class InterruptedException : public std::exception
    {
      public:
        virtual const char *what() const throw() override
        {
            return "interrupted";
        }
    };

} // namespace System
