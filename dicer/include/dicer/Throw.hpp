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

// class MacroResult {
//  public:
// };

// class Macro{
//  public:
//     std::string macroName;
//     std::string recipe;
// };

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
    std::string diceName;
    std::string description;
    std::map<DiceFaceResult, std::string> resultByName;
    DiceFace faces = 0;
};

class GameContext {
 public:
    std::map<std::string, NamedDice> namedDices;
    // std::map<std::string, Macro>     gameMacros;
};

class PlayerContext {
 public:
    std::map<DiceFace, std::map<DiceFaceResult, unsigned int>> occurences;
    // std::map<std::string, Macro> playerMacros;
};

}  // namespace Dicer