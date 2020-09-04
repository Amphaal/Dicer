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
#include <vector>

#include <tao/pegtl.hpp>

#include "dicer/_Base.hpp"
#include "dicer/IDescriptible.hpp"

namespace Dicer {

// TODO comparaison operators

namespace pegtl = tao::pegtl;

class CommandOperator : public IDescriptible {
 public:
    using Order = int;

    virtual const std::string operatorAsString() const = 0;
    virtual const double operate(const double l, const double r) const = 0;
    virtual const Order order() const = 0;
    std::string description() const override {
        return operatorAsString();
    }
};

class MultiplyOperator : public CommandOperator, public pegtl::one< '*' > {
 public:
    const std::string operatorAsString() const override {
        return "*";
    }

    const Order order() const override {
        return 5;
    }

    const double operate(const double l, const double r) const override {
        return l * r;
    }
};

class DivideOperator : public CommandOperator, public pegtl::one< '/' > {
 public:
    const std::string operatorAsString() const override {
        return "/";
    }

    const Order order() const override {
        return 5;
    }

    const double operate(const double l, const double r) const override {
        return l / r;
    }
};

class AdditionOperator : public CommandOperator, public pegtl::one< '+' > {
 public:
    const std::string operatorAsString() const override {
        return "+";
    }

    const Order order() const override {
        return 6;
    }

    const double operate(const double l, const double r) const override {
        return l + r;
    }
};

class SubstractionOperator : public CommandOperator, public pegtl::one< '-' > {
 public:
    const std::string operatorAsString() const override {
        return "-";
    }

    const Order order() const override {
        return 6;
    }

    const double operate(const double l, const double r) const override {
        return l - r;
    }
};

class CommandOperators : public pegtl::sor< MultiplyOperator, DivideOperator, AdditionOperator, SubstractionOperator > {
 public:
    static CommandOperator* get(const std::string &opAsStr) {
        if(!_self) _self = new CommandOperators;
        auto found = _self->_get(opAsStr);
        assert(found);
        return found;
    }

    ~CommandOperators() {
        for(auto op : _ops) {
            delete op;
        }
    }

 private:
    static inline CommandOperators* _self = nullptr;
    std::vector<CommandOperator*> _ops;

    CommandOperators() {
        _ops.push_back(new MultiplyOperator);
        _ops.push_back(new DivideOperator);
        _ops.push_back(new AdditionOperator);
        _ops.push_back(new SubstractionOperator);
    }

    CommandOperator* _get(const std::string &opAsStr) {
        for(auto op : _ops) {
            if(opAsStr == op->operatorAsString()) return op;
        }

        return nullptr;
    }
};

}  // namespace Dicer
