// Siganim - Sign Editor and Animator
// Copyright (C) 2020 Corentin Ferry
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "SignCell.h"
#include <cstddef>
#include "cells/Display.h"

SignCell::SignCell() : parent(nullptr) {
}

const SignCell* SignCell::getParent() const {
    if(this->parent != nullptr)
        return this->parent;
    else throw OrphanNodeException(this);
}

std::ostream& operator<<(std::ostream &strm, const SignCell &s) {
    return s.serialize(strm);
}

bool SignCell::setParent(const SignCell *parent) {
    this->parent = parent;
    this->modified();
    return true;
}

void SignCell::modified() const {
    try {
        this->getParent()->modified();
    } catch(OrphanNodeException& e) {
       // There is no one to notify, then.
    }
}

