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
#include <map>

#include "IResolvable.hpp"
#include "Contexts.hpp"

namespace Dicer {

class ResolvableBase : public IResolvable {
 public:
    virtual ~ResolvableBase() {}
    virtual void resolve(GameContext *gContext, PlayerContext* pContext) {}
};

template<class T>
class Resolvable : public ResolvableBase {
 public:
    virtual ~Resolvable() {}
    T resolved() const {
        return _resolved;
    }

 protected:
    T _resolved;
};

class ResolvableNumber : public Resolvable<double> {
 public:
    explicit ResolvableNumber(double result) {
        _resolved = result;
    }
    ~ResolvableNumber() {}

    std::string resolvedDescription() const override {
        return std::to_string(_resolved);
    }
};

class ResolvableStat : public Resolvable<double> {
 public:
    explicit ResolvableStat(const std::string &statName) : _statName(statName) {}
    ~ResolvableStat() {}

    void resolve(GameContext *gContext, PlayerContext* pContext) override {
        auto &statsValues = pContext->statsValues;

        auto found = statsValues.find(_statName);
        if(found == statsValues.end()) {
            throw std::logic_error("Cannot find associated stat value [" + _statName + "] in the player's context.");
        }

        _resolved = found->second;
    }

    std::string resolvedDescription() const override {
        return _statName + "(" +  std::to_string(_resolved) + ")";
    }

 private:
    std::string _statName;
};

}  // namespace Dicer
