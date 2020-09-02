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

#include <vector>
#include <random>
#include <string>
#include <algorithm>

#include "Exceptions.hpp"
#include "DiceThrow.hpp"
#include "ThrowCommandStack.hpp"
#include "DiceThrowResolvingMethod.hpp"

namespace Dicer {

class FacedDiceThrow : public DiceThrow, public Resolvable<std::vector<DiceFaceResult>> {
 public:
    explicit FacedDiceThrow(int howMany, ThrowCommandStack* stack) : DiceThrow(howMany) {
        _setFacesResolvable(stack);
    }

    FacedDiceThrow(int howMany, int parsedFaces) : DiceThrow(howMany) {
        _setFacesResolvable(new ResolvableNumber(parsedFaces));
    }

    bool isSingleValueResolvable() const override {
        if(howMany() > 1 && !_rm) return false;
        return _facesResolvable->isSingleValueResolvable();
    }

    void resolve(GameContext *gContext, PlayerContext* pContext) override {
        _resolved = DiceThrow::_resolve(gContext, pContext);
        _mightResolveSingleValue();

        ResolvableBase::resolve(gContext, pContext);
    }

    std::string toString() const override {
        return DiceThrow::toString() + _facesResolvable->description();
    }

    std::string description() const override {
        // describe dices results
        std::string throwResults;
        if(_resolved.size()) {
            throwResults = std::accumulate(
                _resolved.begin(),
                _resolved.end(),
                std::string(),
                [](const std::string& a, const DiceFaceResult& b) -> std::string {
                    return a + (a.length() > 0 ? ", " : "") + std::to_string(b);
                }
            );
        } else {
            throwResults = "not resolved";
        }

        // resolving method if any
        std::string singleResolved;
        if(_rm) {
           singleResolved = _rm->funcName() + "(" + _strResolvedSingleValue() + ")";
        }

        // concatenate
        return toString() + "{" + throwResults + "}" + singleResolved;
    }

    void setResolvingMethod(DiceThrowResolvingMethod* method) {
        _rm = method;
    }

 private:
    ResolvableBase* _facesResolvable = nullptr;
    DiceThrowResolvingMethod* _rm = nullptr;

    void _setFacesResolvable(ResolvableBase* resolvable) {
        // can be safely "resolved" if number
        if (auto number = dynamic_cast<ResolvableNumber*>(resolvable)) {
            auto val = number->value();
            if (val <= 1) throw DiceFacesOutOfRange(val);
        }

        _facesResolvable = resolvable;
    }

    DiceFace _resolveFaces(GameContext *gContext, PlayerContext* pContext) override {
        // resolve
        assert(_facesResolvable);
        _facesResolvable->resolve(gContext, pContext);

        // get single value resolved
        assert(_facesResolvable->isSingleValueResolvable());
        auto faces = _facesResolvable->resolvedSingleValue();

        if (faces <= 1) throw DiceFacesOutOfRange(faces);

        return faces;
    }

    void _mightResolveSingleValue() {
        if(!_rm) return;
        _resolvedSingleValue = _rm->resolve(_resolved);
    }
};

}  // namespace Dicer
