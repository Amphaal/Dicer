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

#include "Throw.hpp"

namespace Dicer {

class IResolvable {
 public:
    virtual ~IResolvable() {}
    virtual double resolve(GameContext *gContext, PlayerContext* pContext) const = 0;
};

class Resolvable : public IResolvable {
 public:
    explicit Resolvable(double result) : _result(result) {}
    ~Resolvable() {}

    virtual double resolve(GameContext *gContext, PlayerContext* pContext) const {
        return _result;
    }

 private:
    double _result = 0;
};

class Stat : public IResolvable {
 public:
    explicit Stat(const std::string &statName) : _statName(statName) {}
    ~Stat() {}

    double resolve(GameContext *gContext, PlayerContext* pContext) const {
        auto &statsValues = pContext->statsValues;

        auto found = statsValues.find(_statName);
        if(found == statsValues.end()) {
            throw std::logic_error("Cannot find associated stat value [" + _statName + "] in the player's context.");
        }

        return found->second;
    }

 private:
    std::string _statName;
};

}  // namespace Dicer
