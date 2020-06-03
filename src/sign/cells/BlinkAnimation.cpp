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
#include "BlinkAnimation.h"

BlinkAnimation::BlinkAnimation(SignCell* subject, const unsigned int framesOn,
        const unsigned int framesOff) :
        Animation(subject, framesOn + framesOff), framesOn(framesOn),
        framesOff(framesOff) {
}

unsigned int BlinkAnimation::getFramesOn() const {
    return this->framesOn;
}

unsigned int BlinkAnimation::getFramesOff() const {
    return this->framesOff;
}

void BlinkAnimation::setFramesOn(unsigned int framesOn) {
    this->framesOn = framesOn;
    this->durationFrames = framesOn + this->framesOff;
    this->modified();
}

void BlinkAnimation::setFramesOff(unsigned int framesOff) {
    this->framesOff = framesOff;
    this->durationFrames = this->framesOn + framesOff;
    this->modified();
}

void BlinkAnimation::accept(SignTreeVisitor &visitor) {
    visitor.visit(*this);
}

void BlinkAnimation::accept(ConstSignTreeVisitor &visitor) const {
    visitor.visit(*this);
}

std::ostream& BlinkAnimation::serialize(std::ostream &strm) const {
    return strm << "Blink { " << this->framesOn << "/" << this->framesOff
            << " [" << *(this->subject) << "] }";
}

std::ostream& operator <<(std::ostream &strm, const BlinkAnimation &s) {
    return s.serialize(strm);
}
