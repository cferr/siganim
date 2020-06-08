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

#include <iostream>
#include "Split.h"

Split::Split() : Split(SplitDirection::SPLIT_HORIZONTAL, 10,
        nullptr, nullptr) {

}

Split::Split(enum SplitDirection splitDirection,
        unsigned int splitPos, SignCell* topOrLeftChild,
        SignCell* bottomOrRightChild) :
        splitDirection(splitDirection), splitPos(splitPos),
        topOrLeftChild(topOrLeftChild),
        bottomOrRightChild(bottomOrRightChild) {

    try {
        if(topOrLeftChild != nullptr)
            topOrLeftChild->setParent(this);

        if(bottomOrRightChild != nullptr)
            bottomOrRightChild->setParent(this);

    } catch(SetParentFailedException& e) {
        throw; // Pass on to parent.
    }
}

Split::Split(const Split *a) : splitDirection(a->splitDirection),
        splitPos(a->splitPos) {
    try {
        this->topOrLeftChild = a->getTopOrLeftChild()->copy();
        this->topOrLeftChild->setParent(this);
    } catch(NoSuchChildException& e) {
        this->topOrLeftChild = nullptr;
    }
    try {
        this->bottomOrRightChild = a->getBottomOrRightChild()->copy();
        this->bottomOrRightChild->setParent(this);
    } catch(NoSuchChildException& e) {
        this->bottomOrRightChild = nullptr;
    }
}

SignCell* Split::copy() {
    return new Split(this);
}

Split::~Split() {
    if(this->topOrLeftChild != nullptr)
        delete this->topOrLeftChild;
    if(this->bottomOrRightChild != nullptr)
        delete this->bottomOrRightChild;
}

unsigned int Split::getChildHeight(const SignCell* child) const {
    unsigned int thisHeight = this->getHeight();
    if(this->splitDirection == SplitDirection::SPLIT_HORIZONTAL) {
        if(child == this->topOrLeftChild) {
            if(thisHeight > this->splitPos)
                return this->splitPos;
            else return thisHeight;
        } else if(child == this->bottomOrRightChild) {
            if(thisHeight > this->splitPos)
                return thisHeight - this->splitPos;
            else return 0;
        } else throw NoSuchChildException(this, child);
    } else {
        if(child == this->topOrLeftChild || child == this->bottomOrRightChild)
            return thisHeight;
        else throw NoSuchChildException(this, child);
    }
}

unsigned int Split::getChildWidth(const SignCell* child) const {
    unsigned int thisWidth = this->getWidth();
    if(this->splitDirection == SplitDirection::SPLIT_VERTICAL) {
        if(child == this->topOrLeftChild) {
            if(thisWidth > this->splitPos)
                return this->splitPos;
            else return thisWidth;
        } else if(child == this->bottomOrRightChild) {
            if(thisWidth > this->splitPos)
                return thisWidth - this->splitPos;
            else return 0;
        } else throw NoSuchChildException(this, child);
    } else {
        if(child == this->topOrLeftChild || child == this->bottomOrRightChild)
            return thisWidth;
        else throw NoSuchChildException(this, child);
    }
}

void Split::accept(SignTreeVisitor &visitor) {
    visitor.visit(*this);
}

void Split::accept(ConstSignTreeVisitor &visitor) const {
    visitor.visit(*this);
}

enum Split::SplitDirection Split::getSplitDirection() const {
    return this->splitDirection;
}

bool Split::setSplitDirection(
        enum Split::SplitDirection direction) {
    this->splitDirection = direction;
    this->modified();
    return true;
}

bool Split::setSplitPos(unsigned int splitPos) {
    this->splitPos = splitPos;
    this->modified();
    return true;
}

unsigned int Split::getSplitPos() const {
    return this->splitPos;
}

bool Split::setTopOrLeftChild(SignCell* child) {
    SignCell* oldChild = this->topOrLeftChild;
    this->topOrLeftChild = child;
    if(topOrLeftChild != nullptr)
        try {
            topOrLeftChild->setParent(this);
            if(oldChild != nullptr) {
                oldChild->setParent(nullptr);
            }
        } catch(SetParentFailedException& e) {
            this->topOrLeftChild = oldChild;
            return false;
        }
    this->modified();
    this->structureChanged();
    return true;
}

SignCell* Split::getTopOrLeftChild() const {
    if(this->topOrLeftChild != nullptr)
        return this->topOrLeftChild;
    throw NoSuchChildException(this, nullptr);
}

bool Split::setBottomOrRightChild(SignCell* child) {
    SignCell* oldChild = this->bottomOrRightChild;
    this->bottomOrRightChild = child;
    if(bottomOrRightChild != nullptr)
        try {
            bottomOrRightChild->setParent(this);
            if(oldChild != nullptr) {
                oldChild->setParent(nullptr);
            }
        } catch(SetParentFailedException& e) {
            this->bottomOrRightChild = oldChild;
            return false;
        }
    this->modified();
    this->structureChanged();
    return true;
}

SignCell* Split::getBottomOrRightChild() const {
    if(this->bottomOrRightChild != nullptr)
        return this->bottomOrRightChild;
    throw NoSuchChildException(this, nullptr);
}

unsigned int Split::getHeight() const {
    return this->getParent()->getChildHeight(this);
}

unsigned int Split::getWidth() const {
    return this->getParent()->getChildWidth(this);
}

void Split::callbackDispatch(ConstSignTreeDispatcher *s) const {
    s->dispatchCallback(*this);
}

void Split::callbackDispatch(SignTreeDispatcher *s) {
    s->dispatchCallback(*this);
}

Split::TopOrLeftChildBuilder* Split::topOrLeftChildBuilder() {
    return new TopOrLeftChildBuilder(this);
}

Split::BottomOrRightChildBuilder* Split::bottomOrRightChildBuilder() {
    return new BottomOrRightChildBuilder(this);
}

void Split::deleteChild(SignTree *child) {
    try {
        if(child == this->getTopOrLeftChild()) {
            this->setTopOrLeftChild(nullptr);
            delete child;
        }
    } catch(NoSuchChildException& e) {

    }

    try {
        if(child == this->getBottomOrRightChild()) {
            this->setBottomOrRightChild(nullptr);
            delete child;
        }
    } catch(NoSuchChildException& e) {

    }
}

std::ostream& Split::serialize(std::ostream &strm) const {
    strm << "{ " << this->getWidth() << "x" << this->getHeight() << " +";
    switch(this->splitDirection) {
    case SplitDirection::SPLIT_HORIZONTAL:
        strm << "H";
        break;
    case SplitDirection::SPLIT_VERTICAL:
        strm << "V";
        break;
    case SplitDirection::SPLIT_SW_NE_DIAGONAL:
        strm << "S";
        break;
    case SplitDirection::SPLIT_NW_SE_DIAGONAL:
        strm << "N";
        break;
    }
    strm << this->splitPos << " [" << *(this->topOrLeftChild)
            << ", " << *(this->bottomOrRightChild) << "] }";
    return strm;
}

std::ostream& operator<<(std::ostream &strm, const Split &s) {
    return s.serialize(strm);
}

Split::TopOrLeftChildBuilder::TopOrLeftChildBuilder(Split *split) :
    split(split) {
}

bool Split::TopOrLeftChildBuilder::build(SignCell *child) {
    return this->split->setTopOrLeftChild(child);
}

Split::BottomOrRightChildBuilder::BottomOrRightChildBuilder(Split *split) :
    split(split) {
}

bool Split::BottomOrRightChildBuilder::build(SignCell *child) {
    return this->split->setBottomOrRightChild(child);
}

void Split::deepDetachStructureObserver(SignTreeStructureObserver *observer) {
    this->detachStructureObserver(observer);
    try {
        this->getTopOrLeftChild()->deepDetachStructureObserver(observer);
    } catch(NoSuchChildException& e) { }
        try {
        this->getBottomOrRightChild()->deepDetachStructureObserver(observer);
    } catch(NoSuchChildException& e) { }
}
