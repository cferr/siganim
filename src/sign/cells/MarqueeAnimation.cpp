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

MarqueeAnimation::MarqueeAnimation() : MarqueeAnimation(nullptr, 20, 0,
        Direction::LEFT, 10) {

}

MarqueeAnimation::MarqueeAnimation(SignCell *subject,
        unsigned int durationFrames, const unsigned int initialPhaseFrames,
        enum Direction direction, unsigned int space) :
                Animation(subject, durationFrames, initialPhaseFrames),
                direction(direction), space(space) {
}

MarqueeAnimation::MarqueeAnimation(const MarqueeAnimation *a) :
    Animation(a), direction(a->direction), space(a->space) {
}

SignCell* MarqueeAnimation::copy() {
    return new MarqueeAnimation(this);
}

void MarqueeAnimation::setDirection(enum Direction direction) {
    this->direction = direction;
    this->modified();
}

void MarqueeAnimation::setSpace(unsigned int space) {
    this->space = space;
    this->modified();
}

std::ostream& MarqueeAnimation::serialize(std::ostream &strm) const {
    return strm << "Marquee { " << this->getWidth() << "x" << this->getHeight()
            << " " << ((this->direction == LEFT)?"Left":"Right") <<
            " spacing " << this->space <<
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

unsigned int MarqueeAnimation::getSpace() const {
    return this->space;
}

void MarqueeAnimation::setDurationFrames(unsigned int durationFrames) {
    this->durationFrames = durationFrames;
    this->modified();
}

void MarqueeAnimation::callbackDispatch(ConstSignTreeDispatcher *s) const {
    s->dispatchCallback(*this);
}

void MarqueeAnimation::callbackDispatch(SignTreeDispatcher *s) {
    s->dispatchCallback(*this);
}

json_object* MarqueeAnimation::toJSON() const {
    json_object* ret = json_object_new_object();
    json_object_object_add(ret, "type",
            json_object_new_string("MarqueeAnimation"));
    json_object_object_add(ret, "durationFrames",
            json_object_new_int(this->durationFrames));
    json_object_object_add(ret, "space",
            json_object_new_int(this->space));
    json_object_object_add(ret, "initialPhaseFrames",
            json_object_new_int(this->initialPhaseFrames));

    std::string directionStr;
    switch(this->direction) {
    case MarqueeAnimation::Direction::LEFT:
        directionStr = "left";
        break;
    case MarqueeAnimation::Direction::RIGHT:
        directionStr = "right";
        break;
    }
    json_object_object_add(ret, "direction",
            json_object_new_string(directionStr.c_str()));

    try {
        json_object_object_add(ret, "subject", this->getSubject()->toJSON());
    } catch(NoSuchChildException& e) {

    }

    return ret;
}
