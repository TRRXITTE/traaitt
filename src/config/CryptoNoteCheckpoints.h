// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Bytecoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Bytecoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Bytecoin.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <cstddef>
#include <initializer_list>

namespace CryptoNote
{
    struct CheckpointData
    {
        uint32_t index;
        const char *blockId;
    };


    const std::initializer_list<CheckpointData> CHECKPOINTS = {
        {0, "613646f8d73de696fef75859972b8801ead95070b7ead04c23f63272f49ffb20"},
        {2, "b35bf9fd15098d3ecf077a1ca973be9766fe9bc2692a671b7de6def149963438"},
        {100, "157404420af8463ce8fa1088267cc7d1a95308b8961881e6e1e4dfc95a3ced6f"},
        {100000, "4333517f9a4cad7c55122a84d323acf25f69f2c531c82c52591f38ab40e30447"},
        {145000, "4353959a39835d77c4d688f06da37886e12ffcd434b475639e05576a36935690"},
        {200000, "9c06e9cdae0d4eb01aa99d631f6bd12fa5cfa6baaf4813b6c298b4030d5fa106"},
        {245000, "6a09d5ab1c8518bcd52eb3bb0fbc6b6eed4555d61ac656eb2477fa5ae6829592"},
        {270000, "079c9e2975c6fad2562fb8b77da427d620282587356f49ab0f21a3fc88c3f67c"},
        {500000, "e7edb0c87ee5e7ab70642e9afb8c9b44a04ffbf92583634ff71b9d68f95331b6"},
        {600000, "e617faf540c49f140f3414e042a0d17c603c216c58cf6a698ec01b0d9a6e641e"},
        {700000, "ed58df51bddffa31b4b667599d4ebb8cbefbab610db7207ca59cb3df341d1d42"},
        {800000, "1fd712068a8c81d5c7c1106469bdb8c9bd29194bb5511a797e4e001e2336acd5"},
        {900000, "7883b3eff553bf45e31cebaf6da32b29ffee1dca1409a94a71ab341bd998426b"},
        {1000000, "709602cab8eb1fe2b5a343ffea31fcd4c7e7f847bc6d1c62bdc1da4f67bc0af2"},
        {1100000, "353c82a30793e7407b2ff00508c70e0bd828429be8786d20ead3ba5dc9a32853"},
        {1200000, "56b8539e345cf67c25e73ba96877fc371b1d8aaf376079525ce278b592cc3c16"},
        {1300000, "28c9e6e41229951c3c63eec6f673c5c13d4d90655e83810f336794da69a93e6b"},
        {1400000, "5f13cc4293af4c24c11ad3302ce35a731ae4d0a2e1cee8dce0e1069b307fc66d"},
        {1500000, "b03ce6835c503eee46ea824db55b656f9e9204a6f579b48ddcdd2f2dbb089ac0"},
        {1600000, "c280e995d7eca1aa976fb30c399189354f04097989d7bac031222f98af2acb5e"},
        {1700000, "8ef9b85d0f9882e41c3f88324bc6bc5795986a44d1aa3d88853299110068f141"},
        {1800000, "47b708a8bb4137b28e807ddb87f8af283910993b021348a3b0a3c27ddfea7b76"},
        {1825000, "cdfe7bc7b389d0206599189b3c0f03163ce548fd6faa3d9db599984871a43284"}};
} // namespace CryptoNote
