// Dicer
// Dice throws and Macros parser API
// Copyright (C) 2020-2021 Guillaume Vara

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
#include <limits>

#include "ThrowCommandExtract.hpp"

namespace Dicer {

class Resolver;

struct Resolved {
    friend Resolver;

 public:
    std::string asString() const {
        return hasSingleResult() ? _commandAndResultAsString + " => " + _singleResultAsString : _commandAndResultAsString;
    }

    std::string commandAndResultAsString() const {
        return _commandAndResultAsString;
    }

    bool hasSingleResult() const {
        return _isSingleResolvable;
    }

    double singleResult() const {
        return _singleResult;
    }

    bool isBetween(double val1, double val2) const {
        return _singleResult >= val1 && _singleResult <= val2;
    }

 private:
    std::string _commandAndResultAsString;
    std::string _singleResultAsString;
    bool _isSingleResolvable = false;
    double _singleResult = -1;
};

class Resolver {
 public:
    static Resolved resolve(Dicer::GameContext* gContext, Dicer::PlayerContext* pContext, Dicer::ThrowCommandExtract &extract) {
        // recursive resolve
        extract._master.resolve(gContext, pContext);

        Resolved r;

        // get debug text
        r._commandAndResultAsString = extract.command().signature() + " : " + extract._master.description();

        // if single value resolvable try to get it
        if(extract._master.isSingleValueResolvable()) {
            r._isSingleResolvable = true;
            r._singleResult = extract._master.resolvedSingleValue();
            r._singleResultAsString = ResolvableBase::strResolved(r._singleResult);
        }

        return r;
    }
};

}  // namespace Dicer
