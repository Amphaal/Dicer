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

#include <iostream>
#include <string>
#include <utility>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/analyze.hpp>

#include "DicerPEGTL.hpp"

namespace Dicer {

class Resolver {
 public:
    explicit Resolver(Dicer::GameContext* gContext) : _gContext(gContext) {
        if(!_gContext) throw std::logic_error("Empty game context given as arguement for command parser!");
    }

    Dicer::ThrowCommandResult parseThrowCommand(Dicer::PlayerContext* pContext, const std::string textCommand) {
        if(!pContext) throw std::logic_error("Empty player context given as arguement for parsing command !");

        // throw command
        Dicer::ThrowCommand command {
            _gContext,
            pContext,
            textCommand
        };

        // result
        Dicer::ThrowCommandResult result;

        // parse
        tao::pegtl::memory_input in(command.signature(), "");
        try {
            pegtl::parse<Dicer::PEGTL::grammar, Dicer::PEGTL::action>(in, command, result);
        } catch (const std::logic_error &e) {
            result.setError(e.what());
        } catch (...) {
            result.setError("Unhandled error");
        }

        return result;
    }

 private:
    Dicer::GameContext* _gContext = nullptr;
};

}  // namespace Dicer
