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

#include <functional>
#include <map>
#include <cassert>
#include <string>
#include <vector>

#include "Resolvable.hpp"

namespace Dicer {

// Class that takes care of an operand and an operator stack for
// shift-reduce style handling of operator priority; in a
// reduce-step it calls on the functions contained in the op
// instances to perform the calculation.

class ThrowCommandStack : public IResolvable {
 public:
    ThrowCommandStack() {}
    ~ThrowCommandStack() {
        for(auto resolvablePtr : _resolvables) {
            delete resolvablePtr;
        }
    }

    void push(IResolvable* rslvbl) {
        _resolvables.push_back(rslvbl);
    }

    const std::vector< IResolvable* >& orderedResolvables() const {
        return _resolvables;
    }

    std::string resolvedDescription() const override {
        // assert
        auto rslvblsCount = _resolvables.size();
        assert( rslvblsCount > 1 );
        assert( rslvblsCount % 2 != 0 );

        // populate first
        std::string out;
        for(auto resolvable : _resolvables) {
            out += resolvable->resolvedDescription() + " ";
        }

        out.erase(out.size() - 1, 1);

        return out;
    }

 private:
    std::vector< IResolvable* > _resolvables;
};

}  // namespace Dicer
