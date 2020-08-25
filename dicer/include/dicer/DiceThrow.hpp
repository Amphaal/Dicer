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

class DiceThrow {
 public:
    DiceThrow() {}
    DiceThrow(unsigned int howMany, DiceFace faces) : DiceThrow(howMany) {
        setFaces(faces);
    }
    DiceThrow(unsigned int howMany, NamedDice* associatedNamedDice) : DiceThrow(howMany) {
        setNamedDice(associatedNamedDice);
    }

    unsigned int howMany() const {
        return _howMany;
    }

    DiceFace faces() const {
        return _faces;
    }

    void setFaces(DiceFace faces) {
        if (faces <= 1) throw std::logic_error("A dice face must be > 1");
        _faces = faces;
    }

    void setHowMany(unsigned int howMany) {
        _howMany = howMany;
    }

    void setNamedDice(NamedDice* associatedNamedDice) {
        if (!associatedNamedDice) throw std::logic_error("Named dice associated with throw does not exist");
        _associatedNamedDice = associatedNamedDice;
    }

    NamedDice* namedDice() const {
        return _associatedNamedDice;
    }

 private:
    explicit DiceThrow(unsigned int howMany) : _howMany(howMany) {
        if (!howMany) throw std::logic_error("Number of dices to be thrown should be > 0");
    }

    DiceFace _faces = 0;
    unsigned int _howMany = 0;
    NamedDice* _associatedNamedDice = nullptr;
};

}  // namespace Dicer
