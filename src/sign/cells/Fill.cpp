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

#include "Fill.h"

Fill::Fill(const SignColor &color) : color(color) {
}

const SignColor Fill::getColor() const {
    return this->color;
}

void Fill::setColor(SignColor &color) {
    SignColor c(color);
    this->color = c;
    this->modified();
}

unsigned int Fill::getHeight() const {
    if(this->parent != nullptr) {
        return this->parent->getChildHeight(this);
    } else throw OrphanNodeException(this);
}

unsigned int Fill::getWidth() const {
    if(this->parent != nullptr) {
        return this->parent->getChildWidth(this);
    } else throw OrphanNodeException(this);
}

unsigned int Fill::getChildHeight(const SignCell *child) const {
    throw NoSuchChildException(this, child);
}

unsigned int Fill::getChildWidth(const SignCell *child) const {
    throw NoSuchChildException(this, child);
}

void Fill::accept(SignTreeVisitor &visitor) {
    visitor.visit(*this);
}

void Fill::accept(ConstSignTreeVisitor &visitor) const {
    visitor.visit(*this);
}

bool Fill::setParent(const SignCell *parent) {
    this->parent = parent;
    return true;
}

std::ostream& Fill::serialize(std::ostream &strm) const {
    return strm << "Fill { color = " << this->color << " }";
}

std::ostream& operator <<(std::ostream &strm, const Fill &s) {
    return s.serialize(strm);
}
