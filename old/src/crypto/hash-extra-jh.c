// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2018-2020, The TurtleCoin Developers // Copyright (c) 2020, TRRXITTE inc.
//
// Please see the included LICENSE file for more information.

#include "hash-ops.h"
#include "jh.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

void hash_extra_jh(const void *data, size_t length, char *hash)
{
    int r = jh_hash(HASH_SIZE * 8, data, 8 * length, (uint8_t *)hash);
    if (r)
    {
    }
    assert(SUCCESS == r);
}
