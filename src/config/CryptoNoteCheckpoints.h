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
        {300000, "2fdaeb5a0bc14de9c29cee0363dec4de80260e3b118816d9135db69ed29daa80"},
        {400000, "93966d1314f9e2431bcf39464b18cf61e74f91ae85b84061567126e8e11bfa50"},
        {500000, "ea226b412d18a3afcdfd55e6568215c7040f368363f45483c20dd0f70754ff9f"},
        {600000, "33716bd29dd098dcad042a802fd2a5218b2a6427be226e6a164d65466a4dde08"},
        {700000, "277d3e71b8da3070217305b3f3fbc61e9f3c2935e1ff9f31d3eb4f21722cc50d"},
        {800000, "b1f135b7c75cca68c4be2eee293a4cc455ee18791af7f81065a279634070a0ed"},
        {900000, "d581fe90c4f9be0f40b75363a8011253000273eac10cc9b492215b231ae20ab4"},
        {1000000, "f42142caae87d9479f70511497b8dc7eccb3e7060e06ad77f57f88050a56c8e3"},
        {1100000, "8103268755c46acef4eb55a743898c8c0f9a8ba9b74e65605e2fa6d60c8a519b"},
        {1200000, "a4ce8c0b8bdc9c3a4da44a51bb1140924d6de981b52b8f5354c64bb258fec9bf"},
        {1300000, "d688ec49f7f234b08c1a01020f2efbdf3a23fe2c34db27bfbe22f2ecbb917e64"},
        {1400000, "c6b2e8cf1e281cf572d88b889d71a3233060efbadb045ae011760f950cdf8095"},
        {1500000, "3a2d8c835c4b3ba611db744ce0546b4b1fcc5bb1eff399e5ef7ccf7bc387a6e6"},
        {1575000, "45ed2ad7d847323cacb68e51c7f71b8545cb6dae36577d2d4bd53076c634ca42"}};
} // namespace CryptoNote
