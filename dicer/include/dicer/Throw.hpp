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
#include <vector>
#include <algorithm>
#include <cctype>
#include <locale>

namespace Dicer {

using DiceFace = unsigned int;
using DiceFaceResult = unsigned int;

class NamedDice {
 public:
    NamedDice(const std::string &diceName, const std::string &description, std::map<DiceFaceResult, std::string> resultByName) :
        _diceName(diceName), _description(description), _resultByName(resultByName) {
        if(!diceName.size()) throw std::logic_error("Named dice has no name");
        if(!description.size()) throw std::logic_error("Named dice has no description");
        if(!_resultByName.size()) throw std::logic_error("Named dice map is empty");
    }

    std::string diceName() const {
        return _diceName;
    }

    std::string description() const {
        return _description;
    }

    DiceFace facesCount() {
        return _resultByName.size();
    }

 private:
    std::string _diceName;
    std::string _description;
    std::map<DiceFaceResult, std::string> _resultByName;
};

class ThrowsRepartition {
 public:
    explicit ThrowsRepartition(DiceFace df) : _repartitionOf(df) {
        _generateDefaultWeightedArray();
    }

    // added result weight is reduced by half but cannot be < 1, others are incremented by 1 until they reach their default weight
    void addResult(DiceFaceResult result) {
        _throwsHistory.emplace_back(result);

        // calculate new weight
        auto resultWeight = _weightedArray[result];
        resultWeight = (unsigned int)std::round( .5 * resultWeight);

        // replace it
        _weightedArray[result] = resultWeight;

        // try to increment every other
        for(DiceFaceResult i = 1; i <= _repartitionOf; i++) {
            // skip added result
            if (i == result) continue;

            auto &weight = _weightedArray[i];

            // skip if already at maximum and default state
            if (weight == _repartitionOf) continue;

            weight++;
        }
    }

    const std::map<DiceFaceResult, unsigned int>& weightedArray() const {
        return _weightedArray;
    }

 private:
    DiceFace _repartitionOf = 0;
    std::map<DiceFaceResult, unsigned int> _weightedArray;
    std::vector<DiceFaceResult> _throwsHistory;

    // a face is as strong as the face value by default
    void _generateDefaultWeightedArray() {
        for(DiceFaceResult i = 1; i <= _repartitionOf; i++) {
            _weightedArray[i] = _repartitionOf;
        }
    }
};

class GameContext {
 public:
    std::map<std::string, NamedDice> namedDices;
};

class PlayerContext {
 public:
    std::map<DiceFace, ThrowsRepartition> occurences;
    std::map<std::string, double> statsValues;
};

}  // namespace Dicer
