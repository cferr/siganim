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

Sign::Sign(unsigned int width, unsigned int height) :
    width(width), height(height) {

}

Sign::Sign(unsigned int width, unsigned int height,
        std::vector<SignDisplay*> displays) :
        Sign(width, height) {
    for (SignDisplayVectIt i = displays.begin(); i < displays.end(); ++i) {
        this->addDisplay(*i);
    }
}

Sign::Sign(unsigned int width, unsigned int height,
        std::initializer_list<SignDisplay*> displays) :
        Sign(width, height) {
    for (auto display : displays)
        this->addDisplay(display);
}

Sign::~Sign() {
    // Destroy every child cell
}

unsigned int Sign::getHeight() const {
    return this->height;
}

unsigned int Sign::getWidth() const {
    return this->width;
}

void Sign::accept(SignTreeVisitor &visitor) {
    visitor.visit(*this);
}

void Sign::accept(ConstSignTreeVisitor &visitor) const {
    visitor.visit(*this);
}

bool Sign::addDisplay(SignDisplay *display) {
    displays.push_back(display);
    return true;
}

bool Sign::removeDisplay(SignDisplay *display) {
    bool ret = false;
    for (SignDisplayVectIt i = this->displays.begin(); i < this->displays.end();
            ++i) {
        if (*i == display) {
            this->displays.erase(i);
            ret = true;
        }
    }
    return ret;
}

std::vector<SignDisplay*> Sign::getDisplays() const {
    return this->displays;
}

std::ostream& Sign::serialize(std::ostream &strm) const {
    strm << "Sign { " << this->width << "x" << this->height << " { ";

    SignDisplayVectConstIt i = this->displays.begin();
    while (i < this->displays.end()) {
        strm << (*(*i));
        ++i;
        if (i < this->displays.end())
            strm << ", ";
    }

    strm << " } }";
    return strm;
}

std::ostream& operator<<(std::ostream &strm, const Sign &s) {
    return s.serialize(strm);
}
