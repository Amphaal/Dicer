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

namespace Dicer {

// class MacroResult {

// };

// class Macro{
//  public:
//     std::string macroName;
//     std::string recipe;
// };

using DiceFace = unsigned int;
using DiceFaceResult = unsigned int;
enum DiceResolvingMethod {
    HighestValue,
    LowestValue,
    Aggregate,
    Multiply,
    Random
};

class NamedDice {
    std::string diceName;
    std::string description;
    std::map<DiceFaceResult, std::string> resultByName;
};

class DiceThrow {
    DiceFace faces = 0;
    unsigned int howMany = 0;
    NamedDice* associatedNamedDice = nullptr;
};

class DiceThrowResult {
    DiceThrow* dThrow = nullptr;
    std::vector<DiceFaceResult> throwsValues;
    std::map<DiceFace, DiceFaceResult> repartition;
    double average = 0;
};

class Recipe {
    std::string signature;
    bool resolved = false;
    bool error = true;
    std::vector<std::string> parts;
};

class Throw {
    Recipe* recipe = nullptr;
    double result;
};

class GameContext {
    std::map<std::string, NamedDice> namedDices;
    // std::map<std::string, Macro>     gameMacros;
};

class PlayerContext {
    std::map<DiceFace, std::map<DiceFaceResult, unsigned int>> occurences;
    std::vector<Throw> playerThrows;
    // std::map<std::string, Macro> playerMacros;
};



}  // namespace Dicer
