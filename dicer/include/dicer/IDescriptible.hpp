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

class IDescriptible {
 public:
    virtual ~IDescriptible() {}
    virtual std::string description() const = 0;
};

// For each binary operator known to the calculator we need an
// instance of the following data structure with the priority,
// and a function that performs the calculation. All operators
// are left-associative.

class ResolvableOperation : public IDescriptible {
 public:
    // This enum is used for the order in which the operators are
    // evaluated, i.e. the priority of the operators; a higher
    // number indicates a lower priority.
    using Order = int;

    using Operation = std::function< double( double, double ) >;
    ResolvableOperation(const std::string &sign, Order opOrder, const Operation &operation)
        : _sign(sign), _opOrder(opOrder), _operation(operation) {}

    std::string description() const override {
        return _sign;
    }

    Order opOrder() const {
        return _opOrder;
    }

    double operate(double l, double r) const {
        return _operation(l, r);
    }

 private:
    std::string _sign;
    Order _opOrder;
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
        try_emplace("*", new ResolvableOperation("*", 5, []( const double l, const double r ) { return l * r; }));
        try_emplace("/", new ResolvableOperation("/", 5, []( const double l, const double r ) { return l / r; }));
        try_emplace("+", new ResolvableOperation("+", 6, []( const double l, const double r ) { return l + r; }));
        try_emplace("-", new ResolvableOperation("-", 6, []( const double l, const double r ) { return l - r; }));
    }
};

}  // namespace Dicer
