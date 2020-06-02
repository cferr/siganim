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

#include "MarqueeAnimation.h"

MarqueeAnimation::MarqueeAnimation(SignCell *subject,
        unsigned int durationFrames, enum Direction direction, bool slant,
        unsigned int space) : Animation(subject, durationFrames),
        direction(direction), slant(slant), space(space) {
}

void MarqueeAnimation::setDirection(enum Direction direction) {
    this->direction = direction;
}

void MarqueeAnimation::setSlant(bool slant) {
    this->slant = slant;
}

void MarqueeAnimation::setSpace(unsigned int space) {
    this->space = space;
}

std::ostream& MarqueeAnimation::serialize(std::ostream &strm) const {
    return strm << "Marquee { " << this->getWidth() << "x" << this->getHeight()
            << " " << ((this->direction == LEFT)?"Left":"Right") << " " <<
            ((this->slant)?"Slanted ":"") <<
            "spacing " << this->space <<
            " [" << *(this->subject) << "] }";
}

std::ostream& operator <<(std::ostream &strm, const MarqueeAnimation &s) {
    return s.serialize(strm);
}

void MarqueeAnimation::accept(SignTreeVisitor &visitor) {
    return visitor.visit(*this);
}

void MarqueeAnimation::accept(ConstSignTreeVisitor &visitor) const {
    return visitor.visit(*this);
}

enum MarqueeAnimation::Direction MarqueeAnimation::getDirection() const {
    return this->direction;
}

bool MarqueeAnimation::getSlant() const {
    return this->slant;
}

unsigned int MarqueeAnimation::getSpace() const {
    return this->space;
}

void MarqueeAnimation::setDurationFrames(unsigned int durationFrames) {
    this->durationFrames = durationFrames;
}
