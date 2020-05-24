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

#include "SignCellTree.h"

SignCellTree::SignCellTree(unsigned int height, unsigned int width) :
        height(height), width(width) {

}

SignCellTree::~SignCellTree() {

}

unsigned int SignCellTree::getHeight() const {
    return this->height;
}

unsigned int SignCellTree::getWidth() const {
    return this->width;
}

bool SignCellTree::setHeight(const unsigned int height) {
    return this->resize(height, this->getWidth());
}

bool SignCellTree::setWidth(const unsigned int width) {
    return this->resize(this->getHeight(), width);
}

bool SignCellTree::resize(const unsigned int height, const unsigned int width) {
    unsigned int previousHeight = this->height;
    unsigned int previousWidth = this->width;

    this->height = height;
    this->width = width;

    // Check if we fit in parent...
    if ((this->parent != NULL)) {
        SignTreeType t = this->parent->getType();
        if (t == SignTreeType::SIGN_CELL_NODE) {
            SignCellTree *p = (SignCellTree*) parent;
            if (!(p->checkResize())) {
                this->height = previousHeight;
                this->width = previousWidth;
                return false;
            }
        }
    }
    return true;

}

bool SignCellTree::setParent(const SignTree *parent) {
    if (parent != NULL) {
        SignTreeType t = parent->getType();
        if (t == SignTreeType::SIGN_CELL_NODE
                || t == SignTreeType::SIGN_DISPLAY) {
            this->parent = parent;
            return true;
        } else
            return false;
    } else {
        this->parent = parent; // NULL in this case
        return true;
    }
}

std::ostream& operator<<(std::ostream &strm, const SignCellTree &s) {
    return s.serialize(strm);
}

