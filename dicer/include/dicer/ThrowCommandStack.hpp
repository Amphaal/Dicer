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

// This enum is used for the order in which the operators are
// evaluated, i.e. the priority of the operators; a higher
// number indicates a lower priority.

enum class order : int {};

// For each binary operator known to the calculator we need an
// instance of the following data structure with the priority,
// and a function that performs the calculation. All operators
// are left-associative.

struct op {
    order p;
    std::function< double( double, double ) > operate;
};

struct CommandOperators {
    CommandOperators() {
        // By default we initialise with all binary operators from the C language that can be
        // used on integers, all with their usual priority.

        insert( "*", order( 5 ), []( const double l, const double r ) { return l * r; } );
        insert( "/", order( 5 ), []( const double l, const double r ) { return l / r; } );
        insert( "+", order( 6 ), []( const double l, const double r ) { return l + r; } );
        insert( "-", order( 6 ), []( const double l, const double r ) { return l - r; } );
    }

    // Arbitrary user-defined operators can be added at runtime.

    void insert( const std::string& name, const order p, const std::function< double( double, double ) >& f ) {
        assert( !name.empty() );
        m_ops.try_emplace( name, op{ p, f } );
    }

    [[nodiscard]] const std::map< std::string, op >& ops() const noexcept {
        return m_ops;
    }

 private:
    std::map< std::string, op > m_ops;
};


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

    void push( const op& b ) {
        if(_ops.size() > _resolvables.size()) throw std::logic_error("trying to push more operations than than adequate resolvables");
        _ops.push_back( b );
    }

    void push(IResolvable* rslvbl) {
        if(_resolvables.size() > _ops.size() + 1) throw std::logic_error("trying to push more resolvables than adequate operations");
        _resolvables.push_back(rslvbl);
    }

    IResolvable* latest() const {
        return _resolvables.back();
    }

    double resolve(GameContext *gContext, PlayerContext* pContext) const {
        // assert
        auto rslvblsCount = _resolvables.size();
        assert( rslvblsCount > 1 );
        assert( _ops.size() == rslvblsCount - 1  );

        // populate first
        double result = _resolvables.back()->resolve(gContext, pContext);
        rslvblsCount--;

        // iterate each others
        while(rslvblsCount) {
            auto index = rslvblsCount - 1;
            auto &opPart = _ops[index];
            auto &leftResolvable = _resolvables[index];

            // result is right part
            result = opPart.operate(
                leftResolvable->resolve(gContext, pContext),
                result
            );

            rslvblsCount--;
        }

        return result;
    }

 private:
    std::vector< op > _ops;
    std::vector< IResolvable* > _resolvables;
};

}  // namespace Dicer
