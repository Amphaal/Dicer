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

#include <map>
#include <vector>
#include <numeric>
#include <cmath>
#include <stdexcept>

#include "_Base.hpp"

namespace Dicer {

class ThrowsRepartition {
 public:
    explicit ThrowsRepartition(DiceFace df) : _repartitionOf(df) {
        _generateDefaultWeightedArray();
    }

    // added result weight is reduced by half but cannot be < 1, others are incremented by 1 until they reach their default weight
    DiceFaceResult incorporate(const WeightedSeedResult &wsr) {
        // get dice throw result
        auto result = _getResultFromWeightedSeedResult(wsr);
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

        // update weight count
        _updateWeightCount();

        return result;
    }

    unsigned int weightCount() const {
        return _weightCount;
    }

 private:
    DiceFace _repartitionOf = 0;
    std::map<DiceFaceResult, unsigned int> _weightedArray;
    unsigned int _weightCount = 0;
    std::vector<DiceFaceResult> _throwsHistory;

    // a face is as strong as the face value by default
    void _generateDefaultWeightedArray() {
        for(DiceFaceResult i = 1; i <= _repartitionOf; i++) {
            _weightedArray[i] = _repartitionOf;
        }

        // update weight count
        _updateWeightCount();
    }

    void _updateWeightCount() {
        _weightCount = std::accumulate(
            _weightedArray.begin(),
            _weightedArray.end(),
            (unsigned int)0,
            [](const unsigned int& a, const std::map<int, int>::value_type& b) -> unsigned int {
                return a + b.second;
            }
        );
    }

    DiceFaceResult _getResultFromWeightedSeedResult(const WeightedSeedResult &wsr) {
        auto generated = wsr._v;
        auto low_bound = 0;
        auto high_bound = 0;

        for(auto &i : _weightedArray) {
            // update bounds
            auto &weight = i.second;
            low_bound = high_bound + 1;
            high_bound += weight;

            // check if inbounds
            if(generated >= low_bound && generated <= high_bound) {
                auto &dfr = i.first;
                return dfr;
            }
        }

        throw std::logic_error("Out of bounds WSR");
    }
};

}  // namespace Dicer
