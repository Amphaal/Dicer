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

#include <tao/pegtl/contrib/control_action.hpp>

#include "./Grammar.hpp"

#include "dicer/NamedDiceThrow.hpp"

namespace pegtl = tao::pegtl;

namespace Dicer {

namespace PEGTL {

// The base-case of the class template for the actions, does nothing.

template< typename Rule >
struct action
{};

template<>
struct action< macro > {
    template< typename ActionInput >
    static void apply( const ActionInput& in, Dicer::ThrowCommandExtract& r) {
        // TODO(amphaal) macro calls and nested, check for non recursiveness
        throw MacroNotFound(in.string());
    }
};

template<>
struct action< CommandOperators > {
    template< typename ActionInput >
    static void apply( const ActionInput& in, Dicer::ThrowCommandExtract& r) {
        auto op = CommandOperators::get(in.string());
        r.push(op);
    }
};

template<>
struct action< number > {
    template< typename ActionInput >
    static void apply( const ActionInput& in, Dicer::ThrowCommandExtract& r) {
        // cast to double
        double val = std::atof(in.string().c_str());

        // push number
        r.pushNumber(val);
    }
};

//
// Opening a bracketed expression
//

template<>
struct action< one< '(' > > {
    static void apply0(Dicer::ThrowCommandExtract& r) {
        r.openStack();
    }
};
template<>
struct action< one< ')' > > {
    static void apply0(Dicer::ThrowCommandExtract& r) {
        r.closeStack();
    }
};

//
// all dices throws
//

template<>
struct action< how_many > {
    template< typename ActionInput >
    static void apply(const ActionInput& in, Dicer::ThrowCommandExtract& r) {
        auto howMany = std::stoi(in.string());
        r.setHowManyBuffer(howMany);
    }
};

template<>
struct action< dice_separator > {
    static void apply0(Dicer::ThrowCommandExtract& r) {
        r.setDiceThrowExpected();
    }
};

//
// Trying to build a faced dice throw...
//

template<>
struct action< faces_value > {
    template< typename ActionInput >
    static void apply(const ActionInput& in, Dicer::ThrowCommandExtract& r) {
        // get in situ numeric dice face value
        auto faces = std::stoi(in.string());
        r.pushSimpleFaced(faces);
    }
};

//
// named dices case
//

template<>
struct action< custom_dice_id > {
    template< typename ActionInput >
    static void apply(const ActionInput& in, Dicer::ThrowCommandExtract& r) {
        // search for associated Named Dice
        auto custom_dice_name = in.string();
        auto &namedDices = r.command().gameContext()->namedDices;
        auto found = namedDices.find(custom_dice_name);

        // should be found
        if(found == namedDices.end()) {
            throw std::logic_error("Cannot find associated named dice [" + custom_dice_name + "] in the game context.");
        }

        // define
        auto namedDice = &found->second;

        // generate named dice throw
        r.pushNamed(namedDice, in.string_view());
    }
};

//
// When detecting resolving operator...
//

template<>
struct action< ResolvingMethods > {
    template< typename ActionInput >
    static void apply(const ActionInput& in, Dicer::ThrowCommandExtract& r) {
        auto method = ResolvingMethods::get(in.string());
        r.defineResolvingMethodOnLatestDiceThrow(method, in.string_view());
    }
};

}  // namespace PEGTL

}  // namespace Dicer
