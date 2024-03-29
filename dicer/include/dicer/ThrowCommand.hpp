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

#include "Contexts.hpp"

namespace Dicer {

class ThrowCommand {
 public:
    ThrowCommand() {}
    ThrowCommand(const GameContext* gContext, const PlayerContext* pContext, std::string signature) : _gContext(gContext), _pContext(pContext) {
        if(!gContext) throw std::logic_error("Empty game context provided to throw command");
        if(!pContext) throw std::logic_error("Empty player context provided to throw command");

        // assign
        _signature = signature;
    }

    const std::string& signature() const {
        return _signature;
    }

    const GameContext* gameContext() const {
        return _gContext;
    }

    const PlayerContext* playerContext() const {
        return _pContext;
    }

 private:
    const GameContext* _gContext = nullptr;
    const PlayerContext* _pContext = nullptr;
    std::string _signature;
};

}  // namespace Dicer
