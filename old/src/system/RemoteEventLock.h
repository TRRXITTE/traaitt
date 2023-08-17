// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2018-2020, The TurtleCoin Developers // Copyright (c) 2020, TRRXITTE inc.
//
// Please see the included LICENSE file for more information.

#pragma once

namespace System
{
    class Dispatcher;

    class Event;

    class RemoteEventLock
    {
      public:
        RemoteEventLock(Dispatcher &dispatcher, Event &event);

        ~RemoteEventLock();

      private:
        Dispatcher &dispatcher;

        Event &event;
    };

} // namespace System
