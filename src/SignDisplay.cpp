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

#include "SignDisplay.h"
#include "SignImage.h"

SignDisplay::SignDisplay(unsigned int width, unsigned int height,
        enum SignPixelType type) :
        SignDisplay(width, height, type, NULL) {

}

SignDisplay::SignDisplay(unsigned int width, unsigned int height,
        enum SignPixelType type, SignCell *rootCell) :
        width(width), height(height), displayType(type), rootCell(rootCell),
        parentSign(NULL) {
    if(rootCell != NULL)
        rootCell->setParent(this);
}

SignDisplay::~SignDisplay() {

}

void SignDisplay::accept(SignTreeVisitor &visitor) {
    visitor.visit(*this);
}

SignCell::Type SignDisplay::getType() const {
    return Type::DISPLAY;
}

bool SignDisplay::setParent(const SignCell *parent) {
    return false;
}

bool SignDisplay::setParentSign(const Sign *parent) {
    this->parentSign = parent;
    return true;
}

const Sign* SignDisplay::getParentSign() const {
    return this->parentSign;
}

bool SignDisplay::setRootCell(SignCell *rootCell) {
    this->rootCell = rootCell;
    if(rootCell != NULL)
        rootCell->setParent(this);
    return true;
}

SignCell* SignDisplay::getRootCell() {
    return this->rootCell;
}

enum SignPixelType SignDisplay::getDisplayType() const {
    return this->displayType;
}

bool SignDisplay::setDisplayType(enum SignPixelType displayType) {
    this->displayType = displayType;
    return true;
}

unsigned int SignDisplay::getChildHeight(const SignCell* child) const {
    return this->height;
}

unsigned int SignDisplay::getChildWidth(const SignCell* child) const {
    return this->width;
}

unsigned int SignDisplay::getHeight() const {
    return this->height;
}

unsigned int SignDisplay::getWidth() const {
    return this->width;
}

bool SignDisplay::setHeight(const unsigned int height) {
    return this->resize(height, this->width);
}

bool SignDisplay::setWidth(const unsigned int height) {
    return this->resize(this->height, width);
}

bool SignDisplay::resize(const unsigned int width, const unsigned int height) {
    this->height = height;
    this->width = width;
    // TODO check that children fit
    return true;
}

std::ostream& SignDisplay::serialize(std::ostream &strm) const {
    return strm << "Disp { " << this->width << "x" << this->height << " ("
            << this->displayType << ") : " << *(this->rootCell) << " }";
}

std::ostream& operator<<(std::ostream &strm, const SignDisplay &s) {
    return s.serialize(strm);
}
