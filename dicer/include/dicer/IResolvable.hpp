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
#include <functional>

#include "_Base.hpp"

namespace Dicer {

class IResolvable {
 public:
    virtual ~IResolvable() {}
    virtual std::string resolvedDescription() const = 0;
};

// This enum is used for the order in which the operators are
// evaluated, i.e. the priority of the operators; a higher
// number indicates a lower priority.

enum class order : int {};

// For each binary operator known to the calculator we need an
// instance of the following data structure with the priority,
// and a function that performs the calculation. All operators
// are left-associative.

class ResolvableOperation : public IResolvable {
 public:
    using Operation = std::function< double( double, double ) >;
    ResolvableOperation(const std::string &sign, order opOrder, const Operation &operation)
        : _sign(sign), _opOrder(opOrder), _operation(operation) {}

    std::string resolvedDescription() const override {
        return _sign;
    }
 private:
    std::string _sign;
    order _opOrder;
    std::function< double( double, double ) > _operation;
};

class CommandOperators : public std::map<std::string, ResolvableOperation*> {
 public:
    ~CommandOperators() {
        for(auto op : (*this)) {
            delete op.second;
        }
    }

    static const CommandOperators* get() {
        if(!_get) _get = new CommandOperators;
        return _get;
    }

 private:
    static inline CommandOperators* _get = nullptr;
    CommandOperators() {
        // By default we initialise with all binary operators from the C language that can be
        // used on integers, all with their usual priority.
        try_emplace("*", new ResolvableOperation("*", order( 5 ), []( const double l, const double r ) { return l * r; }));
        try_emplace( "/", new ResolvableOperation("/", order( 5 ), []( const double l, const double r ) { return l / r; }));
        try_emplace( "+", new ResolvableOperation("+", order( 6 ), []( const double l, const double r ) { return l + r; }));
        try_emplace( "-", new ResolvableOperation("-", order( 6 ), []( const double l, const double r ) { return l - r; }));
        try_emplace( ">", new ResolvableOperation("+", order( 7 ), []( const double l, const double r ) { return l > r; }));
        try_emplace( "<", new ResolvableOperation("-", order( 7 ), []( const double l, const double r ) { return l < r; }));
    }
};

}  // namespace Dicer
