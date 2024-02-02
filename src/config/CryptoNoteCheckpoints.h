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
        {270000, "079c9e2975c6fad2562fb8b77da427d620282587356f49ab0f21a3fc88c3f67c"}};
} // namespace CryptoNote
