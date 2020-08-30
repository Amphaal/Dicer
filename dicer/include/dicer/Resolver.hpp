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

#include "ThrowCommandExtract.hpp"

namespace Dicer {

// TODO(amphaal) compute singular value

class Resolver {
 public:
    static std::string asString(const Dicer::ThrowCommandExtract &extract, Dicer::GameContext* gContext, Dicer::PlayerContext* pContext) {
        // recursive resolve
        _resolveStack(&extract.masterStack(), gContext, pContext);

        // get debug text
        return extract.masterStack().resolvedDescription();
    }

 private:
    static void _resolveStack(const ThrowCommandStack* stack, Dicer::GameContext* gContext, Dicer::PlayerContext* pContext) {
        for(auto iresolvables : stack->orderedResolvables()) {
            // if stack, recurse
            auto stack = dynamic_cast<const ThrowCommandStack*>(iresolvables);
            if(stack) _resolveStack(stack, gContext, pContext);

            // if not resolvable, skip
            auto resolvable = dynamic_cast<ResolvableBase*>(iresolvables);
            if(!resolvable) continue;

            // resolve
            resolvable->resolve(gContext, pContext);
        }
    }

    // double computeStackIntoSingleValue(const ThrowCommandStack* stack, GameContext *gContext, PlayerContext* pContext) const {
    //     // assert
    //     auto rslvblsCount = stack->orderedResolvables().size();
    //     assert( rslvblsCount > 1 );
    //     assert( _ops.size() == rslvblsCount - 1  );

    //     // populate first
    //     double result = _resolvables.back()->resolve(gContext, pContext);
    //     rslvblsCount--;

    //     // iterate each others
    //     while(rslvblsCount) {
    //         auto index = rslvblsCount - 1;
    //         auto &opPart = _ops[index];
    //         auto &leftResolvable = _resolvables[index];

    //         // result is right part
    //         result = opPart.operate(
    //             leftResolvable->resolve(gContext, pContext),
    //             result
    //         );

    //         rslvblsCount--;
    //     }

    //     return result;
    // }
};

}  // namespace Dicer
