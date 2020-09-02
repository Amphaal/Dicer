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

#include <vector>
#include <string>
#include <map>

#include "ThrowCommandExtract.hpp"

namespace Dicer {

struct Resolved {
    std::string asString;
    double result = -1;
};

class Resolver {
 public:
    static Resolved resolve(Dicer::GameContext* gContext, Dicer::PlayerContext* pContext, Dicer::ThrowCommandExtract &extract) {
        // recursive resolve
        extract._master.resolve(gContext, pContext);

        // get debug text
        auto descr = extract._master.description();

        Resolved r;

        // if single value resolvable try to get it
        if(extract._master.isSingleValueResolvable()) {
            r.result = extract._master.resolvedSingleValue();
            descr += " => " + ResolvableBase::strResolved(r.result);
        }

        r.asString = descr;

        return r;
    }
};

}  // namespace Dicer
