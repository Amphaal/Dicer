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

#pragma once

#include <string>
#include <functional>
#include <list>

#include <tao/pegtl.hpp>

#include "dicer/ThrowCommandExtract.hpp"
#include "ResolvingMethods.hpp"
#include "Operators.hpp"

// Base template : https://github.com/taocpp/PEGTL/blob/master/src/example/pegtl/calculator.cpp

namespace pegtl = tao::pegtl;

namespace Dicer {

namespace PEGTL {

// A wrapper around the data structures that contain the binary
// operators for the calculator.

// Here the actual grammar starts.

using namespace tao::pegtl;

// The calculator ignores all spaces and comments; space is a pegtl rule
// that matches the usual ascii characters ' ', '\t', '\n' etc. In other
// words, everything that is space is ignored.

struct ignored : space {};

// A number is a non-empty sequence of digits preceded by an optional sign.

struct _number : seq< opt< one< '+', '-' > >, plus< digit > > {};
struct number : _number {};

struct expression;

// A bracketed expression is introduced by a '(' and, in this grammar, must
// proceed with an expression and a ')'.

struct bracket : if_must< one< '(' >, expression, one< ')' > > {};

//
// composition of a dice throw
//

struct DiceThrowProject {
    int howMany;
    int faces;

    template< typename ParseInput >
    DiceThrowProject(const ParseInput&, const Dicer::ThrowCommandExtract&) {}

    template< typename ParseInput >
    void success( const ParseInput&, const Dicer::ThrowCommandExtract&) const {
        auto i = true;
    }
};

struct custom_dice_id : plus< alpha > {};
struct faces_value : _number {};
struct faced_dice : sor<faces_value, bracket> {};
struct faces_part_of_throw : sor<custom_dice_id, faced_dice> {};
struct dice_separator : one< 'd', 'D' > {};
struct how_many : _number {};
    struct dice_throw : seq<how_many, dice_separator, faces_part_of_throw, opt<Dicer::ResolvingMethods>> {};

//
// macro
//

struct macro : plus< alpha > {};

// An atomic expression, i.e. one without operators, is either a number or
// a bracketed expression.

struct atomic : sor< bracket, dice_throw, number, macro > {};

// An expression is a non-empty list of atomic expressions where each pair
// of atomic expressions is separated by an infix operator and we allow
// the rule ignored as padding (before and after every single expression).

struct expression : list< atomic, Dicer::CommandOperators, ignored > {};

// The top-level grammar allows one expression and then expects eof.

struct grammar
    : must< expression, eof >
{};

}  // namespace PEGTL

}  // namespace Dicer
