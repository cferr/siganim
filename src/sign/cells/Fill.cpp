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

Fill::Fill(const Fill *a) : color(a->color) {
}

Fill::Fill() : color(SignColor::off()) {
}

SignCell* Fill::copy() {
    return new Fill(this);
}

const SignColor Fill::getColor() const {
    return this->color;
}

void Fill::setColor(const SignColor &color) {
    SignColor c(color);
    this->color = c;
    this->modified();
}

unsigned int Fill::getHeight() const {
    return this->getParent()->getChildHeight(this);
}

unsigned int Fill::getWidth() const {
    return this->getParent()->getChildWidth(this);
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

void Fill::callbackDispatch(ConstSignTreeDispatcher *s) const {
    s->dispatchCallback(*this);
}

void Fill::callbackDispatch(SignTreeDispatcher *s) {
    s->dispatchCallback(*this);
}

void Fill::deleteChild(SignTree *child) {

}

std::ostream& Fill::serialize(std::ostream &strm) const {
    return strm << "Fill { color = " << this->color << " }";
}

std::ostream& operator <<(std::ostream &strm, const Fill &s) {
    return s.serialize(strm);
}

void Fill::deepDetachStructureObserver(SignTreeStructureObserver *observer) {
    this->detachStructureObserver(observer);
}

json_object* Fill::toJSON() const {
    json_object* ret = json_object_new_object();
    json_object_object_add(ret, "type", json_object_new_string("Fill"));
    json_object_object_add(ret, "color", this->color.toJSON());

    return ret;
}
