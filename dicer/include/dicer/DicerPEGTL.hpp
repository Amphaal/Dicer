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

#include "ThrowCommand.hpp"
#include "ThrowCommandResult.hpp"

namespace pegtl = tao::pegtl;

namespace Dicer {

namespace PEGTL {

struct associative_operator : pegtl::one< '+', '-', '/', '*', '^'> {};
struct dice_separator : pegtl::one< 'd', 'D' > {};
struct dice_throws : pegtl::plus< pegtl::digit > {};

struct macro : pegtl::plus< pegtl::alpha > {};
struct custom_dice_id : pegtl::plus< pegtl::alpha > {};
struct dice_faces : pegtl::plus< pegtl::digit > {};
struct cd_or_df : pegtl::sor<dice_faces, custom_dice_id> {};

struct dice_throw : pegtl::must< dice_throws, dice_separator, cd_or_df> {};

struct anything : tao::pegtl::sor<dice_throw, macro, associative_operator> {};
struct grammar : tao::pegtl::until< tao::pegtl::eof, tao::pegtl::must< anything > > {};

// Class template for user-defined actions that does
// nothing by default.

template< typename Rule >
struct action
{};

// Specialisation of the user-defined action to do
// something when the 'name' rule succeeds; is called
// with the portion of the input that matched the rule.

template<>
struct action< dice_throws > {
    template< typename ActionInput >
    static void apply(const ActionInput& in, Dicer::ThrowCommand& c, Dicer::ThrowCommandResult_Private& r) noexcept {
        auto& current_dt = r.getNext_DT();
        auto howMany = stoi(in.string());
        current_dt.setHowMany(howMany);
    }
};

template<>
struct action< dice_faces > {
    template< typename ActionInput >
    static void apply(const ActionInput& in, Dicer::ThrowCommand& c, Dicer::ThrowCommandResult_Private& r) noexcept {
        auto& current_dt = r.getCurrent_DT();
        auto faces = stoi(in.string());
        current_dt.setFaces(faces);
    }
};

template<>
struct action< custom_dice_id > {
    template< typename ActionInput >
    static void apply(const ActionInput& in, Dicer::ThrowCommand& c, Dicer::ThrowCommandResult_Private& r) {
        auto custom_dice_name = in.string();
        auto& current_dt = r.getCurrent_DT();

        // search for associated Named Dice
        auto &nd_container = c.gameContext()->namedDices;
        auto found = nd_container.find(custom_dice_name);

        // should be found
        if(found == nd_container.end()) {
            throw std::logic_error("Cannot find associated named dice [" + custom_dice_name + "] in the game context.");
        }

        // define
        auto namedDice = &found->second;
        current_dt.setNamedDice(namedDice);
    }
};


}  // namespace PEGTL

}  // namespace Dicer
