// Dicer
// Dice throws and Macros parser API
// Copyright (C) 2020-2021 Guillaume Vara

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

#include <dicer/PEGTL/_.hpp>
#include "specialized/TestUtility.hpp"

TEST_CASE("Must fail tests - simple dice throw", "[Parser]") {
    // oor int
    REQUIRE_THROWS_AS(TestUtility::parse("2000000000000000d7"), std::out_of_range);

    // no negative value on how many
    REQUIRE_THROWS_AS(TestUtility::parse("-1d6"), Dicer::HowManyOutOfRange);

    // no excessive values on how many
    REQUIRE_THROWS_AS(TestUtility::parse(std::to_string(Dicer::MAXIMUM_DICE_HOW_MANY + 1) + "d6"), Dicer::HowManyOutOfRange);

    // dice face should not be too low
    REQUIRE_THROWS_AS(TestUtility::parse("1d1"), Dicer::DiceFacesOutOfRange);
    REQUIRE_THROWS_AS(TestUtility::parse("1d-7"), Dicer::DiceFacesOutOfRange);

    // empty is meaningless
    REQUIRE_THROWS_AS(TestUtility::parse(""), tao::pegtl::parse_error);
    REQUIRE_THROWS_AS(TestUtility::parse("  "), tao::pegtl::parse_error);

    // missing parts
    REQUIRE_THROWS_AS(TestUtility::parse("3d"), tao::pegtl::parse_error);
    REQUIRE_THROWS_AS(TestUtility::parse("D4"), Dicer::MacroNotFound);  // parsed as macro not found

    // grouped expression not allowed on "how many" part
    REQUIRE_THROWS_AS(TestUtility::parse("(3+4)D4"), tao::pegtl::parse_error);  // operator expected after grouping expression

    // missing right or left part of operation
    REQUIRE_THROWS_AS(TestUtility::parse("3 +"), tao::pegtl::parse_error);
    REQUIRE_THROWS_AS(TestUtility::parse("* 3"), tao::pegtl::parse_error);

    // missing opening or closing bracket
    REQUIRE_THROWS_AS(TestUtility::parse("(3 + 4"), tao::pegtl::parse_error);
    REQUIRE_THROWS_AS(TestUtility::parse("3 + 4)"), tao::pegtl::parse_error);

    // just missused brackets
    REQUIRE_THROWS_AS(TestUtility::parse("("), tao::pegtl::parse_error);
    REQUIRE_THROWS_AS(TestUtility::parse(")"), tao::pegtl::parse_error);
    REQUIRE_THROWS_AS(TestUtility::parse("()"), tao::pegtl::parse_error);
    REQUIRE_THROWS_AS(TestUtility::parse("()))"), tao::pegtl::parse_error);

    // using unhandled resolving method
    REQUIRE_THROWS_AS(TestUtility::parse("1d8CAC"), tao::pegtl::parse_error);

    // grouped expression
    REQUIRE_THROWS_AS(TestUtility::parse("1d(1d8+3)"), tao::pegtl::parse_error);  // need to add a space to disambiguate resolving method and operator
}

TEST_CASE("Basic Maths", "[Resolver]") {
    // order of operators
    REQUIRE(TestUtility::pAndR("8 / 2 + 4 * 2 - 8").singleResult() == 4);
    REQUIRE(TestUtility::pAndR("8 + 2 * 4 - 2 / 8").singleResult() == 15.75);
    REQUIRE(TestUtility::pAndR("8 + 2 + 6 - 2 * 4 / 12 - 2 / 8 * 20 - 4").singleResult() == Approx(6.3333));

    // force groupings
    REQUIRE(TestUtility::pAndR("(23)").singleResult() == 23);
    REQUIRE(TestUtility::pAndR("(2 + 4)").singleResult() == 6);
    REQUIRE(TestUtility::pAndR("(8 + 2) * (4 - 2) / 8").singleResult() == 2.5);
    REQUIRE(TestUtility::pAndR("8 / (2 + 4) * (2 - 8)").singleResult() == -8);

    // nesting groupings
    REQUIRE(TestUtility::pAndR("(23 - 12 * (14 - 8 + 2 * (15 / 2)))").singleResult() == -229);
}

TEST_CASE("Dice Throw", "[Parser]") {
    REQUIRE_FALSE(TestUtility::pAndR("3d6").hasSingleResult());

    int i = 100;
    while(i) {
        // basic cases
        REQUIRE(TestUtility::pAndR("1d6").isBetween(1, 6));

        // trying every handled resolving methods
        REQUIRE(TestUtility::pAndR("3d6+").isBetween(3, 18));
        REQUIRE(TestUtility::pAndR("3d6min").isBetween(1, 6));
        REQUIRE(TestUtility::pAndR("3d6max").isBetween(1, 6));

        // meaningless but must work
        REQUIRE(TestUtility::pAndR("1d6+").isBetween(1, 6));

        // grouping expression
        REQUIRE(TestUtility::pAndR("1d(3+3)").isBetween(1, 6));
        REQUIRE(TestUtility::pAndR("1d(1d8 +3)").isBetween(1, 11));
        REQUIRE(TestUtility::pAndR("1d(1d8 +3) * 2").isBetween(2, 22));
        REQUIRE(TestUtility::pAndR("1d(1d8 +3) + 1d4").isBetween(2, 15));

        i--;
    }
}
