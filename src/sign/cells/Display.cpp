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

#include "Display.h"
#include "../Sign.h"

Display::Display(unsigned int width, unsigned int height,
        enum Type type) :
        Display(width, height, type, NULL) {

}

Display::Display(unsigned int width, unsigned int height,
        enum Type type, SignCell *rootCell) :
        width(width), height(height), displayType(type), rootCell(rootCell),
        parentSign(NULL) {
    if(rootCell != nullptr)
        rootCell->setParent(this);
}

Display::~Display() {

}

void Display::accept(SignTreeVisitor &visitor) {
    visitor.visit(*this);
}

void Display::accept(ConstSignTreeVisitor &visitor) const {
    visitor.visit(*this);
}

bool Display::setParent(const SignCell *parent) {
    throw SetParentFailedException(this, parent);
}

bool Display::setParentSign(const Sign *parent) {
    this->parentSign = parent;
    return true;
}

const Sign* Display::getParentSign() const {
    if(this->parentSign != nullptr)
        return this->parentSign;
    else throw OrphanNodeException(this);
}

bool Display::setRootCell(SignCell *rootCell) {
    SignCell* oldRoot = this->rootCell;
    this->rootCell = rootCell;
    if(rootCell != nullptr)
        try {
            rootCell->setParent(this);
            if(oldRoot != nullptr) {
                oldRoot->setParent(nullptr); // Cannot fail.
            }
        } catch(SignCell::SetParentFailedException& e) {
            this->rootCell = oldRoot;
            return false;
        }
    this->modified();
    return true;
}

SignCell* Display::getRootCell() const {
    return this->rootCell;
}

enum Display::Type Display::getDisplayType() const {
    return this->displayType;
}

bool Display::setDisplayType(enum Type displayType) {
    this->displayType = displayType;
    this->modified();
    return true;
}

unsigned int Display::getChildHeight(const SignCell* child) const {
    return this->height;
}

unsigned int Display::getChildWidth(const SignCell* child) const {
    return this->width;
}

unsigned int Display::getHeight() const {
    return this->height;
}

unsigned int Display::getWidth() const {
    return this->width;
}

void Display::setHeight(const unsigned int height) {
    this->resize(height, this->width);
}

void Display::setWidth(const unsigned int height) {
    this->resize(this->height, width);
}

void Display::resize(const unsigned int width, const unsigned int height) {
    this->height = height;
    this->width = width;
    this->modified();
}

std::ostream& Display::serialize(std::ostream &strm) const {
    return strm << "Disp { " << this->width << "x" << this->height << " ("
            << this->displayType << ") : " << *(this->rootCell) << " }";
}

std::ostream& operator<<(std::ostream &strm, const Display &s) {
    return s.serialize(strm);
}

void Display::modified() const {
    this->getParentSign()->modified();
}
