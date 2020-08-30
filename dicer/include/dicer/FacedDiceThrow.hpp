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

#include "Resolvable.hpp"
#include "DiceThrow.hpp"

namespace Dicer {

// TODO() ability to compute dice face

class FacedDiceThrow : public DiceThrow, public Resolvable<std::vector<DiceFaceResult>> {
 public:
     enum GroupingMethod {
        DoNotResolve,
        Aggregate,
        HighestValue,
        LowestValue,
        // Ceiled                      // TODO(stagiaire)
        // HighestValueAddsOneMore     // TODO(stagiaire)
        // LowestValueSubstractOneMore // TODO(stagiaire)
    };

    explicit FacedDiceThrow(unsigned int howMany, DiceFace faces) : DiceThrow(howMany) {
        _setFaces(faces);
    }

    DiceFace faces() const override {
       return _faces;
    }

    void resolve(GameContext *gContext, PlayerContext* pContext) override {
        _resolved = DiceThrow::_resolve(pContext);
        _MightDefineGroupedResolved();
    }

    std::string toString() const override {
        return DiceThrow::toString() + std::to_string(_faces);
    }

    std::string resolvedDescription() const override {
        std::string joinedDescriptor;

        if(_resolved.size()) {
            joinedDescriptor = std::accumulate(
                _resolved.begin(),
                _resolved.end(),
                std::string(),
                [](const std::string& a, const DiceFaceResult& b) -> std::string {
                    return a + (a.length() > 0 ? ", " : "") + std::to_string(b);
                }
            );
        } else {
            joinedDescriptor = "not resolved";
        }

        return toString() + "{" + joinedDescriptor + "}" + _groupedResolvedDescription();
    }

    void setResolvingMethod(GroupingMethod method) {
        _rm = method;
    }

 private:
    DiceFace _faces = 0;
    unsigned int _groupedResolved = 0;
    GroupingMethod _rm = DoNotResolve;

    void _setFaces(DiceFace faces) {
        if (faces <= 1) throw std::logic_error("A dice face must be > 1");
        _faces = faces;
    }

    void _MightDefineGroupedResolved() {
        switch(_rm) {
            case GroupingMethod::Aggregate : {
                _groupedResolved = std::accumulate(_resolved.begin(), _resolved.end(), 0);
            }
            break;

            case GroupingMethod::HighestValue : {
                _groupedResolved = *std::max_element(_resolved.begin(), _resolved.end());
            }
            break;

            case GroupingMethod::LowestValue : {
                _groupedResolved = *std::min_element(_resolved.begin(), _resolved.end());
            }
            break;

            default : {
                // do not set _groupedResolved
            }
            break;
        }
    }

    std::string _groupedResolvedDescription() const {
        std::string descr;

        switch(_rm) {
            case GroupingMethod::Aggregate : {
                descr = "+(";
            }
            break;

            case GroupingMethod::HighestValue : {
                descr = "max(";
            }
            break;

            case GroupingMethod::LowestValue : {
                descr = "min(";
            }
            break;

            case GroupingMethod::DoNotResolve : {
                return descr;
            }
            break;

            default : {
                throw std::logic_error("Unimplemented resolving method for dice throw");
            }
            break;
        }

        descr += std::to_string(_groupedResolved) + ")";
        return descr;
    }
};

}  // namespace Dicer
