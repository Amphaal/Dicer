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

#include <tao/pegtl.hpp>

#include "Throw.h"

#include "DicerPEGTL.hpp"

// https://github.com/taocpp/PEGTL/blob/master/src/example/pegtl/calculator.cpp

int main(int argc, char* argv[]) {
    std::string content;
    std::string command { "1d6" };

    tao::pegtl::string_input in(content, command);

    Dicer::Throw result;
    pegtl::parse<Dicer::PEGTL::grammar, Dicer::PEGTL::action>(in, result);
}
