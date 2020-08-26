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
#include <map>

#include "ThrowCommandStack.hpp"
#include "DiceThrow.hpp"

namespace Dicer {

// Additional layer, a "stack of stacks", to clearly show how bracketed
// sub-expressions can be easily processed by giving them a stack of
// their own. Once a bracketed sub-expression has finished evaluation on
// its stack, the result is pushed onto the next higher stack, and the
// sub-expression's temporary stack is discarded. The top-level calculation
// is handled just like a bracketed sub-expression, on the first stack pushed
// by the constructor.

class ThrowCommandResult {
 public:
    ThrowCommandResult() {
        _stacks.emplace_back(&_master);
    }

    void setError(const std::string &error) {
        _errorString = error;
    }

    CommandOperators& operators() {
        return _operators;
    }

    // open a dice throw stack
    void openStack() {
        auto newStack = new ThrowCommandStack;
        _stacks.back()->push(newStack);
        _stacks.emplace_back(newStack);
    }

    // push into current dice throw stack
    template< typename T >
    void push( const T& t ) {
        assert( !_stacks.empty() );
        _stacks.back()->push( t );
    }

    DiceThrow* latestDiceThrow() const {
        return dynamic_cast<DiceThrow*>(_stacks.back()->latest());
    }

    // close a dice throw stack
    void closeStack() {
        assert( !_stacks.empty() );
        _stacks.pop_back();
    }

    bool hasFailed() const {
        return _errorString.size();
    }

    std::string error() const {
        return _errorString;
    }

 protected:
    std::string _errorString;
    ThrowCommandStack _master;
    CommandOperators _operators;

 private:
    std::vector<ThrowCommandStack*> _stacks;
};

}  // namespace Dicer
