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

#include <exception>
#include <string>

#include "_Base.hpp"

namespace Dicer {

class DicerException : public std::exception {
 public:
    const char* what() const noexcept override {
        return _err.c_str();
    }

 protected:
    void _setErrorMessage(const std::string &errMsg) {
        _err = errMsg;
    }

 private:
    std::string _err;
};

class HowManyOutOfRange : public DicerException {
 public:
    explicit HowManyOutOfRange(int outOfRange) : _outOfRange(outOfRange) {
        _setErrorMessage(std::string("Number of dices to be thrown should be between 1 and ") + std::to_string(MAXIMUM_DICE_HOW_MANY) + ", not " + std::to_string(_outOfRange));
    }

    int outOfRangeNumber() const {
        return _outOfRange;
    }

 private:
    int _outOfRange;
};

class DiceFacesOutOfRange : public DicerException {
 public:
    explicit DiceFacesOutOfRange(double outOfRange) : _outOfRange(outOfRange) {
        _setErrorMessage(std::string("Dice face should be > 1, not ") + std::to_string(_outOfRange));
    }

    double outOfRangeNumber() const {
        return _outOfRange;
    }

 private:
    double _outOfRange;
};

class MacroNotFound : public DicerException {
 public:
    explicit MacroNotFound(const std::string &macroName) : _macroName(macroName) {
        _setErrorMessage(std::string("Macro named [") + _macroName + "] could not be found");
    }

    std::string macroName() const {
        return _macroName;
    }

 private:
    std::string _macroName;
};

}  // namespace Dicer
