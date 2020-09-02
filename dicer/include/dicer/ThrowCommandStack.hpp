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
#include <utility>
#include <list>

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

    void push(IDescriptible* descriptible) {
        // push any component
        _components.push_back(descriptible);

        if (auto resolvable = dynamic_cast<ResolvableBase*>(descriptible)) {
            // if resolvable, check if single value resolvable
            auto isSVR = resolvable->isSingleValueResolvable();
            if(!isSVR) _isSingleValueResolvable = false;

        } else if (auto op = dynamic_cast<ResolvableOperation*>(descriptible)) {
            // if op, track it's order in the stack
            _opsIndexByOrder[op->opOrder()].push_back(_components.size() - 1);
        }
    }

    std::string description() const override {
        // return empty
        std::string out;
        auto componentsCount = _components.size();
        if(!componentsCount) return out;

        // assert
        assert( componentsCount % 2 != 0 );

        // populate first
        for(auto descriptible : _components) {
            out += descriptible->description() + " ";
        }

        out.erase(out.size() - 1, 1);

        return out;
    }

    void resolve(GameContext *gContext, PlayerContext* pContext) override {
        // resolve inner components
        for(auto descriptible : _components) {
            auto resolvable = dynamic_cast<ResolvableBase*>(descriptible);
            if(!resolvable) continue;

            resolvable->resolve(gContext, pContext);
        }

        // then resolve stack expression
        _mayResolveOperations();

        ResolvableBase::resolve(gContext, pContext);
    }

    bool isSingleValueResolvable() const override {
        return _isSingleValueResolvable;
    }

 private:
    bool _isSingleValueResolvable = true;
    std::vector< IDescriptible* > _components;
    std::map<ResolvableOperation::Order, std::vector<int>> _opsIndexByOrder;

    void _mayResolveOperations() {
        // skip if not single value resolvable
        if (!_isSingleValueResolvable) return;

        // assert, make sure components are odd
        auto componentsCount = _components.size();
        assert( componentsCount % 2 != 0 );

        // if only a single component, return asap
        if (componentsCount == 1) {
            auto resolvable = dynamic_cast<ResolvableBase*>(_components.back());
            assert(resolvable);
            _resolvedSingleValue = resolvable->resolvedSingleValue();
            return;
        }

        // try to handle order of operations through buffer...
        using ResultWithIndexesBR = std::pair<double, std::vector<int>>;
        std::map<int, ResultWithIndexesBR*> bufferPtrsByComponentPosition;
        std::list<ResultWithIndexesBR> resultsBuffer;  // use list since ResultWithIndexesBR size is variable

        // iterate through priorities of operator
        for (auto &i : _opsIndexByOrder) {
            auto &indexes = i.second;

            // indexes list by ordered operators
            for(auto y = indexes.begin(); y != indexes.end(); y++) {
                auto opIndex = *y;
                auto lReslvblIndex = opIndex - 1;
                auto rReslvblIndex = opIndex + 1;
                auto op = dynamic_cast<ResolvableOperation*>(_components.at(opIndex));

                assert(op);

                // try to get mot recent results
                auto findResolvedSingleValue = [&bufferPtrsByComponentPosition, &resultsBuffer, this](int index) -> ResultWithIndexesBR* {
                    // find in buffer...
                    auto foundInBuffer = bufferPtrsByComponentPosition.find(index);
                    if(foundInBuffer != bufferPtrsByComponentPosition.end()) {
                        return foundInBuffer->second;
                    }

                    // or from resolvables
                    auto resolvable = dynamic_cast<ResolvableBase*>(_components.at(index));
                    assert(resolvable);

                    // get resolved
                    auto resolved = resolvable->resolvedSingleValue();
                    auto &ptr = resultsBuffer.emplace_back(resolved, std::vector<int>{index});
                    return &ptr;
                };

                // get result
                auto lPart = findResolvedSingleValue(lReslvblIndex);
                auto rPart = findResolvedSingleValue(rReslvblIndex);
                auto result = op->operate(lPart->first, rPart->first);

                // add result to buffer
                resultsBuffer.push_back({result, {}});
                auto resultPtr = &resultsBuffer.back();
                auto &br_indexes = resultPtr->second;

                // push inherited indexes to result
                br_indexes.insert(br_indexes.end(), lPart->second.begin(), lPart->second.end());
                br_indexes.insert(br_indexes.end(), rPart->second.begin(), rPart->second.end());

                // update backreferences values
                for(auto i : br_indexes) {
                    bufferPtrsByComponentPosition[i] = resultPtr;
                }
            }
        }

        // set resolved value as last value from buffer
        _resolvedSingleValue = resultsBuffer.back().first;
    }
};

}  // namespace Dicer
