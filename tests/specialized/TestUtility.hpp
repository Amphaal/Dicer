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

#include <string>

#include <dicer/Parser.hpp>
#include <dicer/Resolver.hpp>

// utility to shorten tests cases
class TestUtility {
 public:
    static Dicer::ThrowCommandExtract parse(const std::string &command) {
        return Dicer::Parser::parseThrowCommand(&_gContext, &_pContext, command);
    }

    static Dicer::Resolved resolve(Dicer::ThrowCommandExtract &extract) {
        return Dicer::Resolver::resolve(&_gContext, &_pContext, extract);
    }

    static Dicer::Resolved pAndR(const std::string &command) {
        auto extract = parse(command);
        return resolve(extract);
    }

    static Dicer::GameContext gameContext() {
        return _gContext;
    }

    static Dicer::PlayerContext playerContext() {
        return _pContext;
    }

 private:
    static inline Dicer::GameContext _gContext;
    static inline Dicer::PlayerContext _pContext;
};
