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

#include "Resolvable.hpp"
#include "DiceThrow.hpp"

namespace Dicer {

class FacedDiceThrow : public DiceThrow, public Resolvable<std::vector<DiceFaceResult>> {
 public:
     enum ResolvingMethod {
        Aggregate,
        HighestValue,
        LowestValue,
        Multiply,
        Random
    };

    explicit FacedDiceThrow(unsigned int howMany, DiceFace faces) : DiceThrow(howMany) {
        _setFaces(faces);
    }

    DiceFace faces() const override {
       return _faces;
    }

    void resolve(GameContext *gContext, PlayerContext* pContext) override {
        _resolved = DiceThrow::_resolve(pContext);
    }

    std::string toString() const override {
        return DiceThrow::toString() + std::to_string(_faces);
    }

    std::string resolvedDescription() const override {
        auto joinedDescriptor = std::accumulate(
            _resolved.begin(),
            _resolved.end(),
            std::string(),
            [](const std::string& a, const DiceFaceResult& b) -> std::string {
                return a + (a.length() > 0 ? ", " : "") + std::to_string(b);
            }
        );

        return toString() + " : {" + joinedDescriptor + "}";
    }

    double resolvedFromMethod() const {
        // what to do with results
        switch(_rm) {
            case ResolvingMethod::Aggregate : {
                return std::accumulate(_resolved.begin(), _resolved.end(), 0);
            }
            break;

            // TODO(amphaal) implement ResolvingMethod switch

            default : {
                throw std::logic_error("Unimplemented resolving method for dice throw");
            }
            break;
        }
    }

 private:
    DiceFace _faces = 0;
    ResolvingMethod _rm = Aggregate;

    void _setFaces(DiceFace faces) {
        if (faces <= 1) throw std::logic_error("A dice face must be > 1");
        _faces = faces;
    }
};

}  // namespace Dicer
