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

#include "NamedDice.hpp"
#include "DiceThrowResolvingMethod.hpp"

namespace Dicer {

// TODO(amphaal) add descriptors for macros

class CommandDescriptorHelper {
 public:
    CommandDescriptorHelper(const std::string_view &sv, const NamedDice* nd) : CommandDescriptorHelper(sv) {
        assert(nd);
        _descr = nd->description();
    }

    CommandDescriptorHelper(const std::string_view &sv, const DiceThrowResolvingMethod* dtrm) : CommandDescriptorHelper(sv) {
        assert(dtrm);
        _descr = dtrm->description();
    }

    const std::string_view& whereInCommand() const {
        return _sv;
    }

    const std::string& description() const {
        return _descr;
    }

 private:
    explicit CommandDescriptorHelper(const std::string_view &sv) : _sv(sv) {}

    std::string_view _sv;
    std::string _descr;
};

}  // namespace Dicer
