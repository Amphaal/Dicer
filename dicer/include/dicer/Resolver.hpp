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

#include "ThrowCommandExtract.hpp"

namespace Dicer {

// TODO(amphaal) compute singular value

class Resolver {
 public:
    static std::string asString(Dicer::GameContext* gContext, Dicer::PlayerContext* pContext, const Dicer::ThrowCommandExtract &extract) {
        // recursive resolve
        _resolveStack(gContext, pContext, &extract._master);

        // get debug text
        auto descr = extract._master.description();

        // if single value resolvable try to get it
        if(extract._master.isSingleValueResolvable()) {
            auto sv = _tryComputeIntoSingleValue(gContext, pContext, extract._master);
            descr += " => " + ResolvableBase::strResolved(sv);
        }

        return descr;
    }

 private:
    static void _resolveStack(Dicer::GameContext* gContext, Dicer::PlayerContext* pContext, const ThrowCommandStack* stack) {
        for(auto descriptible : stack->_components) {
            // if stack, recurse
            auto stack = dynamic_cast<const ThrowCommandStack*>(descriptible);
            if(stack) _resolveStack(gContext, pContext, stack);

            // if not resolvable, skip
            auto resolvable = dynamic_cast<ResolvableBase*>(descriptible);
            if(!resolvable) continue;

            // resolve
            resolvable->resolve(gContext, pContext);
        }
    }

    static double _tryComputeIntoSingleValue(GameContext *gContext, PlayerContext* pContext, const Dicer::ThrowCommandStack &stack) {
        // assert
        auto &resolvables = stack._components;
        auto rslvblsCount = resolvables.size();
        assert( rslvblsCount % 2 != 0 );

        // try to handle order of operations through buffer...
        std::map<int, double> bufferResults;
        double result = 0;

        // iterate through priorities of operator
        for (auto &i : stack._opsIndexByOrder) {
            auto &indexes = i.second;

            // indexes list by ordered operators
            for(auto y = indexes.begin(); y != indexes.end(); y++) {
                auto &opIndex = *y;
                auto lReslvblIndex = opIndex - 1;
                auto rReslvblIndex = opIndex + 1;
                auto op = dynamic_cast<ResolvableOperation*>(stack._components.at(opIndex));

                assert(op);

                // try to get mot recent results
                auto findResolvedSingleValue = [&bufferResults, &stack](int index) -> double {
                    // find in buffer...
                    auto foundInBuffer = bufferResults.find(index);
                    if(foundInBuffer != bufferResults.end()) return foundInBuffer->second;

                    // or from resolvables
                    auto resolvable = dynamic_cast<ResolvableBase*>(stack._components.at(index));
                    assert(resolvable);

                    return resolvable->resolvedSingleValue();
                };

                // get result
                auto lPart = findResolvedSingleValue(lReslvblIndex);
                auto rPart = findResolvedSingleValue(rReslvblIndex);
                result = op->operate(lPart, rPart);

                // store in buffer and override previous if any
                bufferResults[lReslvblIndex] = result;
                bufferResults[rReslvblIndex] = result;
            }
        }

        // return latest operation result
        return result;
    }
};

}  // namespace Dicer
