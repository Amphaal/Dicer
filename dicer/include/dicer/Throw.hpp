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
// enum DiceResolvingMethod {
//     HighestValue,
//     LowestValue,
//     Aggregate,
//     Multiply,
//     Random
// };

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

    DiceFace faces() {
        return _resultByName.size();
    }

 private:
    std::string _diceName;
    std::string _description;
    std::map<DiceFaceResult, std::string> _resultByName;
};

class GameContext {
 public:
    std::map<std::string, NamedDice> namedDices;
};

class PlayerContext {
 public:
    std::map<DiceFace, std::map<DiceFaceResult, unsigned int>> occurences;
    std::map<std::string, double> statsValues;
};

}  // namespace Dicer
