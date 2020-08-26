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

#include "Throw.hpp"

namespace Dicer {

class DiceThrow : public IResolvable {
 public:
    enum Type {
        Unknown,
        Named,
        Arithmetic
    };

    explicit DiceThrow(unsigned int howMany) {
        _setHowMany(howMany);
    }

    unsigned int howMany() const {
        return _howMany;
    }

    DiceFace faces() const {
        switch(_type) {
            case Type::Named: {
                return _associatedNamedDice->facesCount();
            }
            break;

            case Type::Arithmetic: {
                return _faces;
            }
            break;

            default: {
                throw std::logic_error("Dice Throw type must be set");
            }
            break;
        }
    }

    void setFaces(DiceFace faces) {
        if (faces <= 1) throw std::logic_error("A dice face must be > 1");
        _faces = faces;
        _type = Arithmetic;
    }

    void setNamedDice(NamedDice* associatedNamedDice) {
        if (!associatedNamedDice) throw std::logic_error("Named dice associated with throw does not exist");
        _associatedNamedDice = associatedNamedDice;
        _type = Named;
    }

    NamedDice* namedDice() const {
        return _associatedNamedDice;
    }

    // throw dice
    double resolve(GameContext *gContext, PlayerContext* pContext) const {
        // try to find a throw repartition
        auto faces = this->faces();
        auto &occurences = pContext->occurences;
        auto find = pContext->occurences.find(faces);

        // add repartition from dice face if not already existing
        if(find == occurences.end()) {
            occurences.emplace(faces, faces);
        }

        // randomise
        auto &tRepartition = occurences[faces];
        auto result = _randomise(tRepartition);

        // update throw repartition with result
        tRepartition.addResult(result);
        return result;
    }

 private:
    void _setHowMany(unsigned int howMany) {
        if (!howMany) throw std::logic_error("Number of dices to be thrown should be > 0");
        _howMany = howMany;
    }

    // generate a dice throw value from a throws repartition
    unsigned int _randomise(const ThrowsRepartition &tRepartition) const {
        auto &wArray = tRepartition.weightedArray();

        std::random_device rd;                                      // obtain a random number from hardware
        std::mt19937 gen(rd());                                     // seed the generator
        std::uniform_int_distribution<> distr(1, wArray.size());    // define the range according to weighted array

        return distr(gen);
    }

    unsigned int _howMany = 0;
    Type _type = Unknown;
    DiceFace _faces = 0;
    NamedDice* _associatedNamedDice = nullptr;
};

}  // namespace Dicer
