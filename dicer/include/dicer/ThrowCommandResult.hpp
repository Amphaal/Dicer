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
#include <vector>

#include "DiceThrow.hpp"

namespace Dicer {

class ThrowCommandResult_Private {
 public:
    std::vector<DiceThrow> diceThrows() {
        return _throws;
    }

    void setError(const std::string &error) {
        _errorString = error;
    }

    DiceThrow& getCurrent_DT() {
        return _throws.size() ? _throws.back() : getNext_DT();
    }

    DiceThrow& getNext_DT() {
        return _throws.emplace_back();
    }

 protected:
    std::string _errorString;
    std::vector<DiceThrow> _throws;
};

class ThrowCommandResult : private ThrowCommandResult_Private {
 public:
    explicit ThrowCommandResult(ThrowCommandResult_Private&& f) : ThrowCommandResult_Private(f) {}

    std::vector<DiceThrow> diceThrows() const {
        return _throws;
    }

    bool hasFailed() const {
        return _errorString.size();
    }

    std::string error() const {
        return _errorString;
    }
};

}  // namespace Dicer
