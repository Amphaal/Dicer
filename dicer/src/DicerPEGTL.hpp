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

#include <tao/pegtl.hpp>

#include "Throw.h"

namespace pegtl = tao::pegtl;

namespace Dicer {

namespace PEGTL {

struct sign : pegtl::one< '+', '-', '/', '*', '^'> {};
struct dice_throws : pegtl::plus< pegtl::digit > {};
struct dice_separator : pegtl::one< 'd', 'D' > {};
struct dice_faces : pegtl::plus< pegtl::digit > {};

struct dice_throw : pegtl::must< dice_throws, dice_separator, dice_faces> {};

struct grammar : pegtl::must< dice_throws, pegtl::eof > {};

// Class template for user-defined actions that does
// nothing by default.

template< typename Rule >
struct action
{};

// Specialisation of the user-defined action to do
// something when the 'name' rule succeeds; is called
// with the portion of the input that matched the rule.

template<>
struct action< dice_throw > {
    template< typename ParseInput >
    static void apply(const ParseInput& in, std::string& v) {
        v = in.string();
    }
};

}  // namespace PEGTL

}  // namespace Dicer
