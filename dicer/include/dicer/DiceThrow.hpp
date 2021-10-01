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

#include "_Base.hpp"
#include "Exceptions.hpp"
#include "Resolvable.hpp"

namespace Dicer {

class DiceThrow {
 public:
    explicit DiceThrow(int howMany) {
        _setHowMany(howMany);
    }

    unsigned int howMany() const {
        return _howMany;
    }

    virtual std::string toString() const {
        return std::to_string(_howMany) + "d";
    }

 protected:
    // helper to specifically resolve faces component
    virtual DiceFace _resolveFaces(GameContext *gContext, PlayerContext* pContext) = 0;

    std::vector<DiceFaceResult> _resolve(Dicer::GameContext *gContext, PlayerContext* pContext) {
        // try to resolve face component
        auto faces = _resolveFaces(gContext, pContext);

        // try to find a throw repartition
        std::vector<DiceFaceResult> results;
        auto &occurences = pContext->occurences;
        auto find = pContext->occurences.find(faces);

        // add repartition from dice face if not already existing
        if(find == occurences.end()) {
            occurences.try_emplace(faces, faces);
        }

        auto &tRepartition = occurences.find(faces)->second;

        // randomise for how many we must throw
        auto howMany = _howMany;
        while(howMany) {
            auto wsr = _randomise(tRepartition);
            auto result = tRepartition.incorporate(wsr);  // update throw repartition with result
            results.push_back(result);
            howMany--;
        }

        // return results
        return results;
    }

    // generate a weighted dice throw result
    WeightedSeedResult _randomise(const ThrowsRepartition &tRepartition) const {
        auto maxRand = tRepartition.weightCount();

        std::random_device rd;                                      // obtain a random number from hardware
        std::mt19937 gen(rd());                                     // seed the generator
        std::uniform_int_distribution<> distr(1, maxRand);    // define the range according to weighted array

        WeightedSeedResult wsr;
        wsr._v = distr(gen);
        return wsr;
    }

 private:
    unsigned int _howMany = 0;

    void _setHowMany(int howMany) {
        if (howMany < 0 || howMany > MAXIMUM_DICE_HOW_MANY) throw HowManyOutOfRange(howMany);
        _howMany = howMany;
    }
};

}  // namespace Dicer
