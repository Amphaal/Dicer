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

#include "Throw.hpp"

namespace Dicer {

class DiceThrow : protected IResolvable<std::vector<DiceFaceResult>> {
 public:
    explicit DiceThrow(unsigned int howMany) {
        _setHowMany(howMany);
    }

    unsigned int howMany() const {
        return _howMany;
    }

    virtual DiceFace faces() const = 0;

    std::vector<DiceFaceResult> resolve(GameContext *gContext, PlayerContext* pContext) const {
        // try to find a throw repartition
        std::vector<DiceFaceResult> results;
        auto faces = this->faces();
        auto &occurences = pContext->occurences;
        auto find = pContext->occurences.find(faces);

        // add repartition from dice face if not already existing
        if(find == occurences.end()) {
            occurences.emplace(faces, faces);
        }

        auto &tRepartition = occurences[faces];

        // randomise for how many we must throw
        auto howMany = _howMany;
        while(howMany) {
            auto result = _randomise(tRepartition);
            tRepartition.addResult(result);  // update throw repartition with result
            results.push_back(result);
            howMany--;
        }

        // return results
        return results;
    }

 protected:
    // generate a dice throw value from a throws repartition
    DiceFaceResult _randomise(const ThrowsRepartition &tRepartition) const {
        auto &wArray = tRepartition.weightedArray();

        std::random_device rd;                                      // obtain a random number from hardware
        std::mt19937 gen(rd());                                     // seed the generator
        std::uniform_int_distribution<> distr(1, wArray.size());    // define the range according to weighted array

        return distr(gen);
    }

 private:
    unsigned int _howMany = 0;

    void _setHowMany(unsigned int howMany) {
        if (!howMany) throw std::logic_error("Number of dices to be thrown should be > 0");
        _howMany = howMany;
    }
};

class NamedDiceThrow : public DiceThrow, public IResolvable<std::vector<std::string>> {
 public:
    explicit NamedDiceThrow(unsigned int howMany, NamedDice* associatedNamedDice) : DiceThrow(howMany) {
        _setNamedDice(associatedNamedDice);
    }

    DiceFace faces() const {
        return _associatedNamedDice->facesCount();
    }

    NamedDice* namedDice() const {
        return _associatedNamedDice;
    }

    // throw dice
    std::vector<std::string> resolve(GameContext *gContext, PlayerContext* pContext) const {
        // setup search
        auto mRResults = DiceThrow::resolve(gContext, pContext);

        // find associated dice name
        std::vector<std::string> out;
        for(auto &result : mRResults) {
            auto associatedName = _associatedNamedDice->getFaceName(result);
            out.push_back(associatedName);
        }

        return out;
    }

 private:
    NamedDice* _associatedNamedDice = nullptr;

     void _setNamedDice(NamedDice* associatedNamedDice) {
        if (!associatedNamedDice) throw std::logic_error("Named dice associated with throw does not exist");
        _associatedNamedDice = associatedNamedDice;
    }
};


class FacedDiceThrow : public DiceThrow {
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

    DiceFace faces() const {
       return _faces;
    }

    std::vector<DiceFaceResult> resolve(GameContext *gContext, PlayerContext* pContext) const {
        return DiceThrow::resolve(gContext, pContext);
    }

    double resolveFromMethod(GameContext *gContext, PlayerContext* pContext) const {
        auto mRResults = resolve(gContext, pContext);

        // what to do with results
        switch(_rm) {
            case ResolvingMethod::Aggregate : {
                return std::accumulate(mRResults.begin(), mRResults.end(), 0);
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
