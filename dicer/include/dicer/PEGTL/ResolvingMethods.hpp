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
#include <vector>
#include <algorithm>
#include <map>
#include <numeric>

#include <tao/pegtl.hpp>

#include "dicer/_Base.hpp"

namespace Dicer {

//
// resolving method for dices
//

// Ceiled        // TODO(stagiaire)
// CriticalLow   // TODO(stagiaire)
// CriticalHigh  // TODO(stagiaire)
// Critical      // TODO(stagiaire)

using namespace tao::pegtl;

class DiceThrowResolvingMethod {
 public:
    virtual ~DiceThrowResolvingMethod() {}
    virtual const std::string description() const = 0;
    virtual const std::string funcName() const = 0;
    virtual const double resolve(const std::vector<DiceFaceResult> &results) const = 0;
};

class AggregateRM : public DiceThrowResolvingMethod, public tao::pegtl::one< '+' > {
 public:
    const std::string description() const override {
        return "Performs an addition on all the results";
    };
    const std::string funcName() const override {
        return "+";
    };
    const double resolve(const std::vector<DiceFaceResult> &results) const override {
        return std::accumulate(results.begin(), results.end(), 0);
    }
};

class HighestValueRM : public DiceThrowResolvingMethod, public tao::pegtl::string<'m', 'a', 'x'> {
 public:
    const std::string description() const override {
        return "Picks the highest value of throw";
    };
    const std::string funcName() const override {
        return "max";
    };
    const double resolve(const std::vector<DiceFaceResult> &results) const override {
        return *std::max_element(results.begin(), results.end());
    }
};

class LowestValueRM : public DiceThrowResolvingMethod, public tao::pegtl::string<'m', 'i', 'n'> {
 public:
    const std::string description() const override {
        return "Picks the lowest value of throw";
    };
    const std::string funcName() const override {
        return "min";
    };
    const double resolve(const std::vector<DiceFaceResult> &results) const override {
        return *std::min_element(results.begin(), results.end());
    }
};

class ResolvingMethods : public sor< AggregateRM, LowestValueRM, HighestValueRM > {
 public:
    static DiceThrowResolvingMethod* get(const std::string &funcName) {
        if(!_self) _self = new ResolvingMethods;
        auto found = _self->_get(funcName);
        assert(found);
        return found;
    }

    ~ResolvingMethods() {
        for(auto method : _methods) {
            delete method;
        }
    }

 private:
    static inline ResolvingMethods* _self = nullptr;
    std::vector<DiceThrowResolvingMethod*> _methods;

    ResolvingMethods() {
        _methods.push_back(new AggregateRM);
        _methods.push_back(new LowestValueRM);
        _methods.push_back(new HighestValueRM);
    }

    DiceThrowResolvingMethod* _get(const std::string &funcName) {
        for(auto method : _methods) {
            if(funcName == method->funcName()) return method;
        }

        return nullptr;
    }
};

}  // namespace Dicer
