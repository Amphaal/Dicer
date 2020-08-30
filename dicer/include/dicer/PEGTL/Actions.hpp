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
    static void apply( const ActionInput& in, Dicer::ThrowCommand& /*unused*/, Dicer::ThrowCommandExtract& r) {
        // TODO(amphaal) macro calls and nested, check for non recursiveness
        throw std::logic_error("Unimplemented macro functionality");
    }
};

template<>
struct action< number > {
    template< typename ActionInput >
    static void apply( const ActionInput& in, Dicer::ThrowCommand& /*unused*/, Dicer::ThrowCommandExtract& r) {
        // cast to double
        double val = atof(in.string().c_str());

        // push number
        r.push(new ResolvableNumber(val));
    }
};

//
// Opening a bracketed expression
//

template<>
struct action< one< '(' > > {
    static void apply0(Dicer::ThrowCommand& /*unused*/, Dicer::ThrowCommandExtract& r) {
        r.openStack();
    }
};
template<>
struct action< one< ')' > > {
    static void apply0(Dicer::ThrowCommand& /*unused*/, Dicer::ThrowCommandExtract& r) {
        r.closeStack();
    }
};

//
// Trying to build a dice throw...
//

template<>
struct action< how_many > {
    template< typename ActionInput >
    static void apply(const ActionInput& in, Dicer::ThrowCommand& /*unused*/, Dicer::ThrowCommandExtract& r) {
        auto howMany = stoi(in.string());
        r._bufferHowMany = howMany;
    }
};
template<>
struct action< dice_faces > {
    template< typename ActionInput >
    static void apply(const ActionInput& in, Dicer::ThrowCommand& /*unused*/, Dicer::ThrowCommandExtract& r) {
        // generate faced dice throw
        auto faces = stoi(in.string());
        auto fdt = new FacedDiceThrow(r._bufferHowMany, faces);
        r.push(fdt);
    }
};
template<>
struct action< custom_dice_id > {
    template< typename ActionInput >
    static void apply(const ActionInput& in, Dicer::ThrowCommand& c, Dicer::ThrowCommandExtract& r) {
        // search for associated Named Dice
        auto custom_dice_name = in.string();
        auto &namedDices = c.gameContext()->namedDices;
        auto found = namedDices.find(custom_dice_name);

        // should be found
        if(found == namedDices.end()) {
            throw std::logic_error("Cannot find associated named dice [" + custom_dice_name + "] in the game context.");
        }

        // define
        auto namedDice = &found->second;

        // generate named dice throw
        auto ndt = new NamedDiceThrow(r._bufferHowMany, namedDice);
        r.push(ndt);
    }
};

//
// When detecting resolving operator...
//

template<>
struct action< rm_aggregate > {
    template< typename ActionInput >
    static void apply(const ActionInput& in, Dicer::ThrowCommand& /*unused*/, Dicer::ThrowCommandExtract& r) {
        r.latestFDT()->setResolvingMethod(FacedDiceThrow::GroupingMethod::Aggregate);
    }
};
template<>
struct action< rm_lowest_value > {
    template< typename ActionInput >
    static void apply(const ActionInput& in, Dicer::ThrowCommand& /*unused*/, Dicer::ThrowCommandExtract& r) {
        r.latestFDT()->setResolvingMethod(FacedDiceThrow::GroupingMethod::LowestValue);
    }
};
template<>
struct action< rm_highest_value > {
    template< typename ActionInput >
    static void apply(const ActionInput& in, Dicer::ThrowCommand& /*unused*/, Dicer::ThrowCommandExtract& r) {
        r.latestFDT()->setResolvingMethod(FacedDiceThrow::GroupingMethod::HighestValue);
    }
};

}  // namespace PEGTL

}  // namespace Dicer
