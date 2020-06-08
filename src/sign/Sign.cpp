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

#include "Sign.h"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cstdarg>

Sign::Sign() {

}

Sign::Sign(std::vector<Display*> displays) {
    for (SignDisplayVectIt i = displays.begin(); i < displays.end(); ++i) {
        this->addDisplay(*i);
    }
}

Sign::Sign(std::initializer_list<Display*> displays) {
    for (auto display : displays)
        this->addDisplay(display);
}

Sign::Sign(const Sign *a) {
    for(auto display : a->displays) {
        this->addDisplay(new Display(display));
    }
}

Sign::~Sign() {
    // Destroy every child cell
    for (auto display : this->displays)
        delete display;
}

void Sign::accept(SignTreeVisitor &visitor) {
    visitor.visit(*this);
}

void Sign::accept(ConstSignTreeVisitor &visitor) const {
    visitor.visit(*this);
}

bool Sign::addDisplay(Display *display) {
    displays.push_back(display);
    display->setParentSign(this);
    this->modified();
    this->structureChanged();
    return true;
}

bool Sign::removeDisplay(Display *display) {
    bool ret = false;
    for (SignDisplayVectIt i = this->displays.begin(); i < this->displays.end();
            ++i) {
        if (*i == display) {
            this->displays.erase(i);
            ret = true;
        }
    }
    if(ret) {
        this->modified();
        this->structureChanged();
    }
    return ret;
}

std::vector<Display*> Sign::getDisplays() const {
    return this->displays;
}

std::ostream& Sign::serialize(std::ostream &strm) const {
    strm << "Sign { ";

    SignDisplayVectConstIt i = this->displays.begin();
    while (i < this->displays.end()) {
        strm << (*(*i));
        ++i;
        if (i < this->displays.end())
            strm << ", ";
    }

    strm << " }";
    return strm;
}

std::ostream& operator<<(std::ostream &strm, const Sign &s) {
    return s.serialize(strm);
}

void Sign::modified() const {
    this->changed();
}

void Sign::callbackDispatch(ConstSignTreeDispatcher *s) const {
    s->dispatchCallback(*this);
}

void Sign::callbackDispatch(SignTreeDispatcher *s) {
    s->dispatchCallback(*this);
}

Sign::DisplayBuilder::DisplayBuilder(Sign *sign) : sign(sign) {
}

bool Sign::DisplayBuilder::build(Display *display) {
    return this->sign->addDisplay(display);
}

Sign::DisplayBuilder* Sign::displayBuilder() {
    return new DisplayBuilder(this);
}

void Sign::deleteChild(SignTree *child) {
    for(auto i = this->displays.begin(); i < this->displays.end(); ++i) {
        if(*i == child) {
            this->removeDisplay(*i);
            delete *i;
        }
    }
}

void Sign::deepDetachStructureObserver(SignTreeStructureObserver *observer) {
    this->detachStructureObserver(observer);
    for(auto display : this->displays)
        display->detachStructureObserver(observer);
}
