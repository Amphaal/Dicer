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

#include "Throw.hpp"
#include "ThrowCommandStack.hpp"

namespace Dicer {

// Additional layer, a "stack of stacks", to clearly show how bracketed
// sub-expressions can be easily processed by giving them a stack of
// their own. Once a bracketed sub-expression has finished evaluation on
// its stack, the result is pushed onto the next higher stack, and the
// sub-expression's temporary stack is discarded. The top-level calculation
// is handled just like a bracketed sub-expression, on the first stack pushed
// by the constructor.

class ThrowCommandExtract {
 public:
    explicit ThrowCommandExtract(Dicer::PlayerContext* pContext) : _pContext(pContext) {
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

    const ThrowCommandStack& masterStack() const {
        return _master;
    }

    PlayerContext * playerContext() const {
        return _pContext;
    }

    unsigned int _bufferHowMany = 0;

 protected:
    std::string _errorString;
    CommandOperators _operators;  // operators before stack, because of destructors
    ThrowCommandStack _master;

 private:
    std::vector<ThrowCommandStack*> _stacks;
    Dicer::PlayerContext* _pContext = nullptr;
};

}  // namespace Dicer
