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

namespace pegtl = tao::pegtl;

namespace Dicer {

namespace PEGTL {

// Parsing rule that matches a literal "Hello, ".

struct prefix
    : pegtl::string< 'H', 'e', 'l', 'l', 'o', ',', ' ' >
{};

// Parsing rule that matches a non-empty sequence of
// alphabetic ascii-characters with greedy-matching.

struct name
    : pegtl::plus< pegtl::alpha >
{};

// Parsing rule that matches a sequence of the 'prefix'
// rule, the 'name' rule, a literal "!", and 'eof'
// (end-of-file/input), and that throws an exception
// on failure.

struct grammar
    : pegtl::must< prefix, name, pegtl::one< '!' >, pegtl::eof >
{};

// Class template for user-defined actions that does
// nothing by default.

template< typename Rule >
struct action
{};

// Specialisation of the user-defined action to do
// something when the 'name' rule succeeds; is called
// with the portion of the input that matched the rule.

template<>
struct action< name > {
    template< typename ParseInput >
    static void apply(const ParseInput& in, std::string& v) {
        v = in.string();
    }
};

}  // namespace PEGTL

}  // namespace Dicer
