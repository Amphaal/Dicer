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
#include <list>
#include <functional>
#include <map>
#include <vector>

#include <tao/pegtl.hpp>

#include "ThrowCommand.hpp"
#include "ThrowCommandResult.hpp"

namespace pegtl = tao::pegtl;

namespace Dicer {

namespace PEGTL {

// https://github.com/taocpp/PEGTL/blob/master/src/example/pegtl/calculator.cpp

// A wrapper around the data structures that contain the binary
// operators for the calculator.

// Here the actual grammar starts.
using namespace tao::pegtl;

    struct macro : plus< alpha > {};
    struct custom_dice_id : plus< alpha > {};
    struct dice_faces : plus< digit > {};
    struct df_or_cd : sor<dice_faces, custom_dice_id> {};
    struct dice_separator : one< 'd', 'D' > {};
    struct how_many : plus< digit > {};
struct dice_throw : must< how_many, dice_separator, df_or_cd> {};

// The calculator ignores all spaces and comments; space is a pegtl rule
// that matches the usual ascii characters ' ', '\t', '\n' etc. In other
// words, everything that is space is ignored.

struct ignored : space {};

// Since the binary operators are taken from a runtime data structure
// (rather than hard-coding them into the grammar), we need a custom
// rule that attempts to match the input against the current map of
// operators.

struct infix {
    using rule_t = ascii::any::rule_t;

    template< apply_mode,
                rewind_mode,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
    static bool match( ParseInput& in, Dicer::ThrowCommand& c, Dicer::ThrowCommandResult& r, States&&... /*unused*/ ) {
        // Look for the longest match of the input against the operators in the operator map.

        return match( in, r, std::string() );
    }

 private:
    template< typename ParseInput >
    static bool match( ParseInput& in, Dicer::ThrowCommandResult& r, std::string t ) {
        auto &b = r.operators();

        if( in.size( t.size() + 1 ) > t.size() ) {
            t += in.peek_char( t.size() );
            const auto i = b.ops().lower_bound( t );

            if( i != b.ops().end() ) {
                if( match( in, r, t ) ) {
                    return true;
                }

                if( i->first == t ) {
                    // While we are at it, this rule also performs the task of what would
                    // usually be an associated action: To push the matched operator onto
                    // the operator stack.
                    r.push( i->second );
                    in.bump( t.size() );
                    return true;
                }
            }
        }

        return false;
    }
};

// A number is a non-empty sequence of digits preceded by an optional sign.

struct number : plus< digit > {};

struct expression;

// A bracketed expression is introduced by a '(' and, in this grammar, must
// proceed with an expression and a ')'.

struct bracket
    : if_must< one< '(' >, expression, one< ')' > >
{};

// An atomic expression, i.e. one without operators, is either a number or
// a bracketed expression.

struct atomic : sor< number, bracket, dice_throw, macro > {};

// An expression is a non-empty list of atomic expressions where each pair
// of atomic expressions is separated by an infix operator and we allow
// the rule ignored as padding (before and after every single expression).

struct expression : list< atomic, infix, ignored > {};

// The top-level grammar allows one expression and then expects eof.

struct grammar
    : must< expression, eof >
{};

// After the grammar we proceed with the additional actions that are
// required to let our calculator actually do something.

// The base-case of the class template for the actions, does nothing.

template< typename Rule >
struct action
{};

// This action will be called when the number rule matches; it converts the
// matched portion of the input to a double and pushes it onto the operand
// stack.

template<>
struct action< number > {
    template< typename ActionInput >
    static void apply( const ActionInput& in, Dicer::ThrowCommand& /*unused*/, Dicer::ThrowCommandResult& r) {
        // cast to double
        double val = atof(in.string().c_str());

        // push number
        r.push(new Resolvable(val));
    }
};

// The actions for the brackets call functions that create, and collect
// a temporary additional stack for evaluating the bracketed expression.

template<>
struct action< one< '(' > > {
    static void apply0(Dicer::ThrowCommand& /*unused*/, Dicer::ThrowCommandResult& r) {
        r.openStack();
    }
};

template<>
struct action< one< ')' > > {
    static void apply0(Dicer::ThrowCommand& /*unused*/, Dicer::ThrowCommandResult& r) {
        r.closeStack();
    }
};

template<>
struct action< how_many > {
    template< typename ActionInput >
    static void apply(const ActionInput& in, Dicer::ThrowCommand& /*unused*/, Dicer::ThrowCommandResult& r) {
        auto howMany = stoi(in.string());
        auto diceThrow = new DiceThrow(howMany);
        r.push(diceThrow);
    }
};

template<>
struct action< dice_faces > {
    template< typename ActionInput >
    static void apply(const ActionInput& in, Dicer::ThrowCommand& /*unused*/, Dicer::ThrowCommandResult& r) {
        // get current dice throw
        auto diceThrow = r.latestDiceThrow();
        assert(diceThrow);

        // set faces
        auto faces = stoi(in.string());
        diceThrow->setFaces(faces);
    }
};

template<>
struct action< custom_dice_id > {
    template< typename ActionInput >
    static void apply(const ActionInput& in, Dicer::ThrowCommand& c, Dicer::ThrowCommandResult& r) {
        // get current dice throw
        auto diceThrow = r.latestDiceThrow();
        assert(diceThrow);

        // search for associated Named Dice
        auto custom_dice_name = in.string();
        auto &nd_container = c.gameContext()->namedDices;
        auto found = nd_container.find(custom_dice_name);

        // should be found
        if(found == nd_container.end()) {
            throw std::logic_error("Cannot find associated named dice [" + custom_dice_name + "] in the game context.");
        }

        // define
        auto namedDice = &found->second;
        diceThrow->setNamedDice(namedDice);
    }
};

}  // namespace PEGTL

}  // namespace Dicer
