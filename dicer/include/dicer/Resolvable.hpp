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
#include <map>
#include <cstring>

#include "IDescriptible.hpp"
#include "Contexts.hpp"

namespace Dicer {

class ResolvableBase : public IDescriptible {
 public:
    virtual ~ResolvableBase() {}

    virtual void resolve(GameContext *gContext, PlayerContext* pContext) {
        _beenResolved = true;  // basically only tag as resolved
    }
    virtual bool isSingleValueResolvable() const = 0;

    bool haveBeenResolved() const {
        return _beenResolved;
    }

    double resolvedSingleValue() const {
        return _resolvedSingleValue;
    }

    static std::string strResolved(double val) {
        char buffer[32];
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%g", val);
        return buffer;
    }

 protected:
    double _resolvedSingleValue = 0;

    std::string _strResolvedSingleValue() const {
        return strResolved(_resolvedSingleValue);
    }

 private:
    bool _beenResolved = false;
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

class ResolvableNumber : public ResolvableBase {
 public:
    explicit ResolvableNumber(double result) {
        _resolvedSingleValue = result;
    }
    ~ResolvableNumber() {}

    std::string description() const override {
        return _strResolvedSingleValue();
    }

    double value() const {
        return _resolvedSingleValue;
    }

    bool isSingleValueResolvable() const override {
        return true;
    }
};

class ResolvableStat : public ResolvableBase {
 public:
    explicit ResolvableStat(const std::string &statName) : _statName(statName) {}
    ~ResolvableStat() {}

    void resolve(GameContext *gContext, PlayerContext* pContext) override {
        auto &statsValues = pContext->statsValues;

        auto found = statsValues.find(_statName);
        if(found == statsValues.end()) {
            throw std::logic_error("Cannot find associated stat value [" + _statName + "] in the player's context.");
        }

        _resolvedSingleValue = found->second;

        ResolvableBase::resolve(gContext, pContext);
    }

    std::string description() const override {
        return _statName + "(" +  _strResolvedSingleValue() + ")";
    }

    bool isSingleValueResolvable() const override {
        return true;
    }

 private:
    std::string _statName;
};

}  // namespace Dicer
