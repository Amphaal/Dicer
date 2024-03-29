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

#include <string>
#include <vector>
#include <map>
#include <utility>

#include "ThrowCommandStack.hpp"
#include "FacedDiceThrow.hpp"
#include "NamedDiceThrow.hpp"
#include "CommandDescriptorHelper.hpp"
#include "ThrowCommand.hpp"

namespace Dicer {

// Additional layer, a "stack of stacks", to clearly show how bracketed
// sub-expressions can be easily processed by giving them a stack of
// their own. Once a bracketed sub-expression has finished evaluation on
// its stack, the result is pushed onto the next higher stack, and the
// sub-expression's temporary stack is discarded. The top-level calculation
// is handled just like a bracketed sub-expression, on the first stack pushed
// by the constructor.

class Resolver;

class ThrowCommandExtract {
 public:
    friend class Resolver;

    ThrowCommandExtract(const GameContext* gContext, const PlayerContext* pContext, std::string signature) : _command(gContext, pContext, signature) {
        _stacks.emplace_back(&_master);
    }

    const Dicer::ThrowCommand& command() const {
        return _command;
    }

    //
    //
    //

    // open a dice throw stack
    void openStack() {
        auto newStack = new ThrowCommandStack;

        if(_diceExpected) {
            // if dice is expected, add faced dice throw
            auto fdt = new FacedDiceThrow(_bufferHowMany, newStack);
            push(fdt);
        } else {
            // else, classic push
            push(newStack);
        }

        _stacks.emplace_back(newStack);
    }

    // push into current dice throw stack
    template< typename T >
    void push( const T& t) {
        assert( !_stacks.empty() );
        _stacks.back()->push( t );

        // reset dice throw expectancy
        auto dt = dynamic_cast<DiceThrow*>(t);
        if(dt) {
            _diceExpected = false;
            _bufferHowMany = 0;
        }
    }
    void pushSimpleFaced(int parsedFace) {
        auto fdt = new FacedDiceThrow(_bufferHowMany, parsedFace);
        _latestFDT = fdt;
        push(fdt);
    }
    void pushNamed(const NamedDice* associatedNamedDice, const std::string_view &sv) {
        auto ndt = new NamedDiceThrow(_bufferHowMany, associatedNamedDice);
        push(ndt);

        // add to tracker
        _tracker.emplace_back(sv, associatedNamedDice);
    }
    void pushNumber(double number) {
        auto rNumber = new ResolvableNumber(number);
        push(rNumber);
    }

    // close a dice throw stack
    void closeStack() {
        assert( !_stacks.empty() );
        _stacks.pop_back();
    }

    void setHowManyBuffer(int howMany) {
        _bufferHowMany = howMany;
    }

    void setDiceThrowExpected() {
        _diceExpected = true;
    }

    void defineResolvingMethodOnLatestDiceThrow(DiceThrowResolvingMethod *rm, const std::string_view &sv) {
        assert(_latestFDT);
        assert(rm);
        _latestFDT->setResolvingMethod(rm);

        // add to tracker
        _tracker.emplace_back(sv, rm);
    }

    //
    //
    //

 private:
    Dicer::ThrowCommand _command;
    std::vector<CommandDescriptorHelper> _tracker;
    std::vector<ThrowCommandStack*> _stacks;
    FacedDiceThrow* _latestFDT = nullptr;
    ThrowCommandStack _master;

    int _bufferHowMany = 0;
    bool _diceExpected = false;
};

}  // namespace Dicer
