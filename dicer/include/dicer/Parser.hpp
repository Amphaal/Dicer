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

#pragma once

#include <iostream>
#include <string>
#include <utility>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/analyze.hpp>
#include <tao/pegtl/contrib/trace.hpp>

#include "PEGTL/_.hpp"

namespace Dicer {

class Parser {
 public:
    static Dicer::ThrowCommandExtract parseThrowCommand(const Dicer::GameContext* gContext, const Dicer::PlayerContext* pContext, const std::string &textCommand) {
        // extraction
        Dicer::ThrowCommandExtract extract {
            gContext,
            pContext,
            textCommand
        };

        // parse
        tao::pegtl::memory_input in(extract.command().signature(), "");
        pegtl::parse<Dicer::PEGTL::grammar, Dicer::PEGTL::action>(in, extract);

        return extract;
    }
};

}  // namespace Dicer
