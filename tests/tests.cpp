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

#include "dicer/PEGTL/_.hpp"
#include "specialized/TestParser.hpp"

// TEST_CASE("Must fail tests - how many parts", "[Parser]") {
//     // command throw
//     REQUIRE(TestParser::parse("0d6").hasFailed());
//     REQUIRE(TestParser::parse("1d1").hasFailed());
//     REQUIRE(TestParser::parse("-1d4").hasFailed());
//     REQUIRE(TestParser::parse("1D0").hasFailed());
//     REQUIRE(TestParser::parse("1D-7").hasFailed());
//     REQUIRE(TestParser::parse("3d").hasFailed());
//     REQUIRE(TestParser::parse("D4").hasFailed());
//     REQUIRE(TestParser::parse("").hasFailed());
//     REQUIRE(TestParser::parse("  ").hasFailed());
// }

TEST_CASE("+ Resolution next to a + operator", "[Parser]") {
    auto extract2 = TestParser::parse("4d8++1");
    REQUIRE_FALSE(extract2.error());
}

// 2d6+/2

// TEST_CASE("+ Resolution next to a + operator", "[Parser]") {
//     auto extract = TestParser::parse("4d8++1");
//     REQUIRE_FALSE(extract.error());
// }
