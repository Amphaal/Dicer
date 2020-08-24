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

#include <iostream>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/analyze.hpp>

#include "DicerPEGTL.hpp"

// https://github.com/taocpp/PEGTL/blob/master/src/example/pegtl/calculator.cpp

int main(int argc, char* argv[]) {
    // prepare
    Dicer::PlayerContext pContext;
    Dicer::GameContext gContext;

    // test named dice
    Dicer::NamedDice nd {
        "Force",
        "Force dice",
        {
            { 1, "Weak" },
            { 2, "Strong" },
            { 3, "Unpredictable" }
        }
    };
    gContext.namedDices.emplace(nd.diceName, nd);

    // throw command
    Dicer::ThrowCommand command {
        &gContext,
        &pContext,
        "1dForce"
    };

    // result
    Dicer::ThrowCommandResult result;

    // parse
    tao::pegtl::memory_input in(command.signature(), "");
    pegtl::parse<Dicer::PEGTL::grammar, Dicer::PEGTL::action>(in, command, result);

    auto i = true;
}
