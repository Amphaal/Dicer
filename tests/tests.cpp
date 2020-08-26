// Dicer
// Dice throws and Macros parser API
// Copyright (C) 2020 Guillaume Vara

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Any graphical resources available within the source code may
// use a different license and copyright : please refer to their metadata
// for further details. Graphical resources without explicit references to a
// different license and copyright still refer to this GPL.

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "dicer/DicerPEGTL.hpp"
#include "dicer/Resolver.hpp"

// TEST_CASE("Test parsing simple dice throw", "[Dice]") {
//     // prepare
//     Dicer::PlayerContext pContext;
//     Dicer::GameContext gContext;

//     // resolver
//     Dicer::Resolver resolver(&gContext);

//     // command throw
//     auto result = resolver.parseThrowCommand(&pContext, "1d6");
//     REQUIRE_FALSE(result.hasFailed());
//     REQUIRE(result.diceThrows()[0].howMany() == 1);
//     REQUIRE(result.diceThrows()[0].faces() == 6);
// }

// TEST_CASE("Test parsing named dice throw", "[Dice]") {
//     // prepare
//     Dicer::PlayerContext pContext;
//     Dicer::GameContext gContext;

//     // named dice
//     Dicer::NamedDice nd {
//         "Force",
//         "Force dice",
//         {
//             { 1, "Weak" },
//             { 2, "Strong" },
//             { 3, "Unpredictable" }
//         }
//     };
//     gContext.namedDices.emplace(nd.diceName(), nd);

//     // resolver
//     Dicer::Resolver resolver(&gContext);

//     // command throw
//     auto result = resolver.parseThrowCommand(&pContext, "2dForce");
//     REQUIRE_FALSE(result.hasFailed());
//     REQUIRE(result.diceThrows()[0].howMany() == 2);
//     REQUIRE(result.diceThrows()[0].namedDice());
// }

TEST_CASE("t", "[]") {
    // prepare
    Dicer::PlayerContext pContext;
    Dicer::GameContext gContext;

    // resolver
    Dicer::Resolver resolver(&gContext);

    // command throw
    auto result = resolver.parseThrowCommand(&pContext, "3 + 2 + 4");
    auto i = true;
}

// TEST_CASE("Must fail", "[Dice]") {
//     // prepare
//     Dicer::PlayerContext pContext;
//     Dicer::GameContext gContext;

//     // resolver
//     Dicer::Resolver resolver(&gContext);

//     // command throw
//     REQUIRE(resolver.parseThrowCommand(&pContext, "1d6 2d8").hasFailed());
//     REQUIRE(resolver.parseThrowCommand(&pContext, "0d6").hasFailed());
//     REQUIRE(resolver.parseThrowCommand(&pContext, "1d1").hasFailed());
//     REQUIRE(resolver.parseThrowCommand(&pContext, "-1d4").hasFailed());
//     REQUIRE(resolver.parseThrowCommand(&pContext, "1D0").hasFailed());
//     REQUIRE(resolver.parseThrowCommand(&pContext, "1D-7").hasFailed());
//     REQUIRE(resolver.parseThrowCommand(&pContext, "3d").hasFailed());
//     REQUIRE(resolver.parseThrowCommand(&pContext, "D4").hasFailed());
//     REQUIRE(resolver.parseThrowCommand(&pContext, "").hasFailed());
// }
