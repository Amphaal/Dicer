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

class Resolver;

class ThrowCommandStack : public ResolvableBase {
 public:
    friend class Resolver;

    ThrowCommandStack() {}
    ~ThrowCommandStack() {
        for(auto descriptible : _components) {
            auto op = dynamic_cast<ResolvableOperation*>(descriptible);
            if(!op) delete descriptible;
        }
    }

    void push(IDescriptible* rslvbl) {
        _components.push_back(rslvbl);

        auto op = dynamic_cast<ResolvableOperation*>(rslvbl);
        if (!op) return;

        _opsIndexByOrder[op->opOrder()].push_back(_components.size() - 1);
    }

    std::string description() const override {
        // return empty
        std::string out;
        auto rslvblsCount = _components.size();
        if(!rslvblsCount) return out;

        // assert
        assert( rslvblsCount % 2 != 0 );

        // populate first
        for(auto descriptible : _components) {
            out += descriptible->description() + " ";
        }

        out.erase(out.size() - 1, 1);

        return out;
    }

    void resolve(GameContext *gContext, PlayerContext* pContext) override {
        for(auto descriptible : _components) {
            auto resolvable = dynamic_cast<ResolvableBase*>(descriptible);
            if(!resolvable) continue;

            resolvable->resolve(gContext, pContext);
        }

        ResolvableBase::resolve(gContext, pContext);
    }

    bool isSingleValueResolvable() const override {
        for(auto descriptible : _components) {
            auto resolvable = dynamic_cast<ResolvableBase*>(descriptible);
            if(!resolvable) continue;

            auto isSVR = resolvable->isSingleValueResolvable();
            if(!isSVR) return false;
        }

        return true;
    }

 private:
    std::vector< IDescriptible* > _components;
    std::map<ResolvableOperation::Order, std::vector<int>> _opsIndexByOrder;
};

}  // namespace Dicer
