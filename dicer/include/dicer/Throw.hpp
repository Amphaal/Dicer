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
enum DiceResolvingMethod {
    HighestValue,
    LowestValue,
    Aggregate,
    Multiply,
    Random
};

class NamedDice {
 public:
    std::string diceName;
    std::string description;
    std::map<DiceFaceResult, std::string> resultByName;
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

class DiceThrow {
 public:
    DiceFace faces = 0;
    unsigned int howMany = 0;
    NamedDice* associatedNamedDice = nullptr;
};

class ThrowCommandResult {
 public:
    bool error = false;
    DiceThrow& getCurrent_DT() {
        return _throws.size() ? _throws.back() : getNext_DT();
    }
    DiceThrow& getNext_DT() {
        return _throws.emplace_back();
    }
 private:
    std::vector<DiceThrow> _throws;
};

class ThrowCommand {
 public:
    ThrowCommand(GameContext* gContext, PlayerContext* pContext, std::string signature) : gContext(gContext), pContext(pContext) {
        // trim signature from spaces
        signature.erase(
            std::remove_if(signature.begin(), signature.end(), ::isspace),
            signature.end()
        );

        // assign
        _signature = signature;
    }
    GameContext* gContext = nullptr;
    PlayerContext* pContext = nullptr;
    std::string& signature() {
        return _signature;
    }
 private:
    std::string _signature;
};

}  // namespace Dicer
