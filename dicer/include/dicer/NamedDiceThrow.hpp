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

#include <vector>
#include <random>
#include <string>

#include "Resolvable.hpp"
#include "DiceThrow.hpp"

namespace Dicer {

class NamedDiceThrow : public DiceThrow, public Resolvable<std::vector<std::string>> {
 public:
    explicit NamedDiceThrow(int howMany, const NamedDice* associatedNamedDice) : DiceThrow(howMany) {
        _setNamedDice(associatedNamedDice);
    }

    const NamedDice* namedDice() const {
        return _associatedNamedDice;
    }

    bool isSingleValueResolvable() const override {
        return false;
    }

    // throw dice
    void resolve(GameContext *gContext, PlayerContext* pContext) override {
        // setup search
        auto mRResults = DiceThrow::_resolve(gContext, pContext);

        _resolved.clear();

        // find associated dice name
        for(auto &result : mRResults) {
            auto associatedName = _associatedNamedDice->getFaceName(result);
            _resolved.push_back(associatedName);
        }

        ResolvableBase::resolve(gContext, pContext);
    }

    std::string toString() const override {
        return DiceThrow::toString() + _associatedNamedDice->diceName();
    }

    std::string description() const override {
        std::string joinedDescriptor;

        if(_resolved.size()) {
            joinedDescriptor = std::accumulate(
                _resolved.begin(),
                _resolved.end(),
                std::string(),
                [](const std::string& a, const std::string& b) -> std::string {
                    return a + (a.length() > 0 ? ", " : "") + b;
                }
            );
        } else {
            joinedDescriptor = "not resolved";
        }

        return toString() + "{" + joinedDescriptor + "}";
    }

 private:
    const NamedDice* _associatedNamedDice = nullptr;

    void _setNamedDice(const NamedDice* associatedNamedDice) {
        if (!associatedNamedDice) throw std::logic_error("Named dice associated with throw does not exist");
        _associatedNamedDice = associatedNamedDice;
    }

    DiceFace _resolveFaces(GameContext *gContext, PlayerContext* pContext) override {
        return _associatedNamedDice->facesCount();
    }
};

}  // namespace Dicer
