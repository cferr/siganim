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

#include "SignCellSplit.h"

#include <iostream>

SignCellSplit::SignCellSplit(enum SplitDirection splitDirection,
        unsigned int splitPos, SignCell* topOrLeftChild,
        SignCell* bottomOrRightChild) :
        splitDirection(splitDirection), splitPos(splitPos),
        topOrLeftChild(topOrLeftChild),
        bottomOrRightChild(bottomOrRightChild) {

    if(topOrLeftChild != NULL)
        topOrLeftChild->setParent(this);

    if(bottomOrRightChild != NULL)
        bottomOrRightChild->setParent(this);

}

SignCellSplit::~SignCellSplit() {

}

unsigned int SignCellSplit::getChildHeight(const SignCell* child) const {
    unsigned int thisHeight = this->getHeight();
    if(this->splitDirection == SplitDirection::SPLIT_HORIZONTAL) {
        if(child == this->topOrLeftChild)
            return this->splitPos;
        else if(child == this->bottomOrRightChild)
            return thisHeight - this->splitPos;
        else return 0; // This is not our child
    } else return thisHeight;
}

unsigned int SignCellSplit::getChildWidth(const SignCell* child) const {
    unsigned int thisWidth = this->getWidth();
    if(this->splitDirection == SplitDirection::SPLIT_VERTICAL) {
        if(child == this->topOrLeftChild)
            return this->splitPos;
        else if(child == this->bottomOrRightChild)
            return thisWidth - this->splitPos;
        else return 0; // This is not our child
    } else return thisWidth;
}

SignCell::Type SignCellSplit::getType() const {
    return Type::CELL_SPLIT;
}

void SignCellSplit::accept(SignTreeVisitor &visitor) {
    visitor.visit(*this);
}

enum SignCellSplit::SplitDirection SignCellSplit::getSplitDirection() const {
    return this->splitDirection;
}

bool SignCellSplit::setSplitDirection(
        enum SignCellSplit::SplitDirection direction) {
    this->splitDirection = direction;
    return true;
}

bool SignCellSplit::setSplitPos(unsigned int splitPos) {
    this->splitPos = splitPos;
    return true;
}

unsigned int SignCellSplit::getSplitPos() const {
    return this->splitPos;
}

bool SignCellSplit::setTopOrLeftChild(SignCell* child) {
    this->topOrLeftChild = child;
    if(topOrLeftChild != NULL)
        topOrLeftChild->setParent(this);
    return true;
}

SignCell* SignCellSplit::getTopOrLeftChild() const {
    return this->topOrLeftChild;
}

bool SignCellSplit::setBottomOrRightChild(SignCell* child) {
    this->bottomOrRightChild = child;
    if(bottomOrRightChild != NULL)
        bottomOrRightChild->setParent(this);
    return true;
}

SignCell* SignCellSplit::getBottomOrRightChild() const {
    return this->bottomOrRightChild;
}

bool SignCellSplit::setParent(const SignCell *parent) {
    this->parent = parent;
    return true;
}

unsigned int SignCellSplit::getHeight() const {
    if(this->parent != NULL) {
        return this->parent->getChildHeight(this);
    } else return 0; // Orphan node
}

unsigned int SignCellSplit::getWidth() const {
    if(this->parent != NULL) {
        return this->parent->getChildWidth(this);
    } else return 0; // Orphan node
}

std::ostream& SignCellSplit::serialize(std::ostream &strm) const {
    strm << "{ " << this->getWidth() << "x" << this->getHeight() << " +";
    switch(this->splitDirection) {
    case SplitDirection::SPLIT_HORIZONTAL:
        strm << "H";
        break;
    case SplitDirection::SPLIT_VERTICAL:
        strm << "V";
        break;
    }
    strm << this->splitPos << " [" << *(this->topOrLeftChild)
            << ", " << *(this->bottomOrRightChild) << "] }";
    return strm;
}

std::ostream& operator<<(std::ostream &strm, const SignCellSplit &s) {
    return s.serialize(strm);
}
